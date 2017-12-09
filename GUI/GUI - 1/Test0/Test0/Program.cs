using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

using CySmart.DongleCommunicator.API;
using CySmart.Common.Base.Compatibility.Net20;
using System.Threading;
using System.Windows.Forms;

namespace Test0
{
          	
	class ConsoleCopy : IDisposable
{
  
  FileStream fileStream;
  StreamWriter fileWriter;
  TextWriter doubleWriter;
  TextWriter oldOut;
  
  class DoubleWriter : TextWriter
  {

    TextWriter one;
    TextWriter two;

    public DoubleWriter(TextWriter one, TextWriter two)
    {
      this.one = one;
      this.two = two;
    }

    public override Encoding Encoding
    {
      get { return one.Encoding; }
    }

    public override void Flush()
    {
      one.Flush();
      two.Flush();
    }

    public override void Write(char value)
    {
      one.Write(value);
      two.Write(value);
    }

  }

  public ConsoleCopy(string path)
  {
    oldOut = Console.Out;

    try
    {
      fileStream = File.Create(path);

      fileWriter = new StreamWriter(fileStream);
      fileWriter.AutoFlush = true;

      doubleWriter = new DoubleWriter(fileWriter, oldOut);
    }
    catch (Exception e)
    {
      Console.WriteLine("Cannot open file for writing");
      Console.WriteLine(e.Message);
      return;
    }
    Console.SetOut(doubleWriter);
  }

  public void Dispose()
  {
    Console.SetOut(oldOut);
    if (fileWriter != null)
    {
      fileWriter.Flush();
      fileWriter.Close();
      fileWriter = null;
    }
    if (fileStream != null)
    {
      fileStream.Close();
      fileStream = null;
    }
  }

}
	
	
	
	
	#region NotificationLogger

    /// <summary>
    /// Simple class which logs all received notifications to Console
    /// </summary>
    class NotificationLogger : IDisposable
    {
        const char BLANK = ' ';
    	const char DOT = '.';
    	const char X = 'x';
    	const int cMaxLineChars = 256;
    	const int cHalf = cMaxLineChars / 2;
   		static char[] LINE = new char[cMaxLineChars];
   		//int count = 0;
   		//static int[] y;
    	
    	#region Setup Constants

        // Change the COM port name to the COM port of your CySmart dongle
        const string COM_PORT_NAME = "COM28";

        // Bluetooth address of the peer device you want to connect to
        static readonly CyBleBdAddress PEER_DEVICE_BD_ADDR = 
            new CyBleBdAddress(0x00A050D08793, CyBleBdAddressType.PUBLIC_ADDRESS);

        // CCCD attribute handle of the characteristic whose notification needs to be monitored
        const ushort CHARACTERISTIC_TO_MONITOR_CCCD_HANDLE = 0x0013;

        #endregion

        #region consts

        // Value to enable notification
        const ushort ENABLE_NOTIFICATION = 0x0001;

        // Value to disable notification
        const ushort DISABLE_NOTIFICATION = 0x0000;

        #endregion

        #region members

        // CySmart dongle communicator instance
        ICySmartDongleCommunicator m_communicator;

        // BLE manager callback
        BleMgrCb m_bleMgrCb;

        // Peer BLE device
        ICyBleDevice m_peerDevice;

        // Device callback
        DeviceCb m_deviceCb;

        // GATT client callback
        GattClientCb m_gattClientCb;

        #endregion

        #region props

        /// <summary>
        /// Gets the BLE manager
        /// </summary>
        ICyBleMgr BleMgr
        {
            get { return m_communicator.BleMgr; }
        }

        /// <summary>
        /// Gets the GATT client
        /// </summary>
        ICyGattClient GattClient
        {
            get { return m_peerDevice.GattClient; }
        }

        #endregion

        #region ctor

        public NotificationLogger()
        {
            m_communicator = null;
            m_deviceCb = null;
            m_bleMgrCb = null;
            m_peerDevice = null;
            m_gattClientCb = null;
        }

        #endregion

        #region public methods

        /// <summary>
        /// Connect to CySmart BLE dongle
        /// </summary>
        public CyApiErr ConnectToDongle(CyDongleInfo info)
        {
            CySmartDongleMgr dongleMgr = CySmartDongleMgr.GetInstance();
            CyApiErr err = dongleMgr.TryGetCySmartDongleCommunicator(info, out m_communicator);
            if (err.IsOK)
            {
                m_bleMgrCb = new BleMgrCb();
                err = BleMgr.RegisterBleMgrCallback(m_bleMgrCb);
            }

            return err;
        }

        /// <summary>
        /// Connect to peer BLE device
        /// </summary>
        public CyApiErr ConnectToBleDevice(CyBleBdAddress deviceAddr)
        {
            AutoResetEvent sync = new AutoResetEvent(false);
            CyApiErr err = CyApiErr.OK;

            // Set the connection handler
            m_bleMgrCb.ConnectionHandler = (result, status) =>
            {
                if (status == CyStatus.BLE_STATUS_OK)
                {
                    m_peerDevice = result.Device;

                    // Register the device callback
                    m_deviceCb = new DeviceCb();
                    m_peerDevice.RegisterCallback(m_deviceCb);

                    // Register the GATT client callback
                    m_gattClientCb = new GattClientCb();
                    m_peerDevice.GattClient.RegisterCallback(m_gattClientCb);
                    RegisterNotificationLogger();
                }
                else
                {
                    err = new CyApiErr("Failed to connect to the peer device. Reason: " + status.ToString());
                }

                sync.Set();
            };            

            // Initiate connection request to the peer BLE device
            err = BleMgr.Connect(new CyConnectInfo(deviceAddr, new CyBleConnectionSettings()));
            if (err.IsOK)
                sync.WaitOne();

            return err;
        }

        /// <summary>
        /// Start logging
        /// </summary>
        public CyApiErr StartLogging()
        {
            // Enable notification
            return StartStopLogging(BitConverter.GetBytes(ENABLE_NOTIFICATION));
        }

        /// <summary>
        /// Stop logging
        /// </summary>
        public CyApiErr StopLogging()
        {
            // Disable notification
            return StartStopLogging(BitConverter.GetBytes(DISABLE_NOTIFICATION));
        }

        #endregion

        #region private

        /// <summary>
        /// Start or stop monitoring helper
        /// </summary>
        private CyApiErr StartStopLogging(params byte[] data)
        {
            AutoResetEvent sync = new AutoResetEvent(false);
            CyApiErr err = CyApiErr.OK;

            // Setup the descriptor write handler
            m_gattClientCb.DescriptorWriteHandler = (CyConnectResult, status) =>
            {
                if (status != CyStatus.BLE_STATUS_OK)
                    err = new CyApiErr("Failed to start/stop notification monitoring. Reason: " + status.ToString());

                sync.Set();
            };

            // Initiate write descriptor request to the CCCD
            err = GattClient.WriteDescriptor(new CyGattWriteInfo(CHARACTERISTIC_TO_MONITOR_CCCD_HANDLE, data));
            if (err.IsOK)
                sync.WaitOne();

            return err;
        }

        /// <summary>
        /// Register notification logger
        /// </summary>
        private void RegisterNotificationLogger()
        {
            // Setup the characteristic changed handler
            m_gattClientCb.CharacteristicChangedHandler = (info) =>
            {
                // Log all received notifications
                //Console.WriteLine("[{0}] Handle: {1:X4}, Value: {2}",
                    //DateTime.Now.ToString("HH:mm:ss:fff"),
                    //info.Handle,
                    //GetPrintableValue(info.Value));
                    //Console.WriteLine("{0}",GetPrintableValue(info.Value));
                    
                    
        
            		           		
            		int loc;
            		
            		string str = GetPrintableValue(info.Value);
            		str = str.Substring(1,1);            		
            		int intTemp = Convert.ToInt32(str, 16);
				    //loc = (int)Math.Round(intTemp* cHalf) + cHalf;
					loc = intTemp;
				    LINE[loc] = X;
				    //y[count] = loc - 129;
				    //count = count + 1;
                	Console.WriteLine(LINE);
                	for (int i = 0; i < cMaxLineChars; i++)
            		{
                		LINE[i] = BLANK;
            		}
                	LINE[cHalf] = DOT; // for horizontal axis
            		
            
            };
        }

        /// <summary>
        /// Gets a printable string from the value byte array
        /// </summary>
        private string GetPrintableValue(params byte[] value)
        {
            StringBuilder sb = new StringBuilder();
            foreach (var item in value)
                sb.AppendFormat("{0} ", item.ToString("X2"));

            return sb.ToString().Trim();
        }

        #endregion

        #region IDisposable

        /// <summary>
        /// Dispose the notification monitor instance
        /// </summary>
        public void Dispose()
        {
            if(m_communicator != null)
            {
                CyApiErr err = CySmartDongleMgr.GetInstance().CloseCommunicator(m_communicator);
                if (err.IsNotOK)
                    Console.WriteLine(err.Message);

                m_communicator = null;
            }
        }

        #endregion
        [STAThread]
        #region Main

        static void Main(string[] args)
        {
                  	
        	using (NotificationLogger logger = new NotificationLogger())
            {
                using (var cc = new ConsoleCopy("mylogfile.txt"))
  				{
    				/*
                	for (int i = 0; i < cMaxLineChars; i++)
            		{
                		LINE[i] = DOT;
            		}      
                    Console.WriteLine(LINE);
            		for (int i = 0; i < cMaxLineChars; i++)
            		{
                		LINE[i] = BLANK;
            		}
                	LINE[cHalf] = DOT; // for "horizontal" axis
					*/                	
					//Application.EnableVisualStyles();
    				//Application.Run(new Form());
                	Console.WriteLine("Connecting to the dongle at {0} ...", COM_PORT_NAME);
            	
            	// Change the dongle type to match with the dongle you need to connect
                CyApiErr err = logger.ConnectToDongle(new CyDongleInfo(COM_PORT_NAME, CyDongleInfo.CySmartDongleType.CY5670));
                if (err.IsNotOK)
                {
                    Console.WriteLine(err.Message);
                    return;
                }

                Console.WriteLine("Connecting to peer device: [0x{0:X12},{1}] ...", PEER_DEVICE_BD_ADDR.Address, PEER_DEVICE_BD_ADDR.AddressType);
                err = logger.ConnectToBleDevice(PEER_DEVICE_BD_ADDR);
                if (err.IsNotOK)
                {
                    Console.WriteLine(err.Message);
                    return;
                }

                Console.WriteLine("Enable notification ...");
                err = logger.StartLogging();
                if (err.IsNotOK)
                {
                    Console.WriteLine(err.Message);
                    return;
                }

                Console.WriteLine("Press any key to stop logging");
                Console.ReadKey();

                Console.WriteLine("Disable notification ...");
                err = logger.StopLogging();
                if (err.IsNotOK)
                {
                    Console.WriteLine(err.Message);
                    return;
                }
    				
  				}      	
				     	
            	         	              
            	      
                                            
            
            }
            
            
        }

        #endregion
    }

    #endregion

    #region BleMgrCb

    /// <summary>
    /// BLE manager Callback class
    /// </summary>
    class BleMgrCb : CyBleMgrCallback
    {
        /// <summary>
        /// Gets/Sets the connection handler
        /// </summary>
        public CySmart.Common.Base.Compatibility.Net20.Action<CyConnectResult, CyStatus> ConnectionHandler { get; set; }

        public override void OnConnected(CyConnectResult result, CyStatus status)
        {
            if (ConnectionHandler != null)
                ConnectionHandler(result, status);
        }
    }

    #endregion

    #region DeviceCb

    /// <summary>
    /// Device callback class
    /// </summary>
    class DeviceCb : CyBleDeviceCallback
    {
        // override callback methods if you need to support pairing.
        // Refer to the CySmart API reference guide
    }

    #endregion

    #region GattClientCb

    /// <summary>
    /// GATT client callback class
    /// </summary>
    class GattClientCb : CyGattClientCallback
    {
        /// <summary>
        /// Gets/Sets the descriptor write handler
        /// </summary>
        public CySmart.Common.Base.Compatibility.Net20.Action<CyGattWriteResult, CyStatus> DescriptorWriteHandler { get; set; }

        /// <summary>
        /// Gets/Sets the characteristic changed handler
        /// </summary>
        public Action<CyCharacteristicChangedInfo> CharacteristicChangedHandler { get; set; }

        public override void OnDescriptorWrite(CyGattWriteResult result, CyStatus status)
        {
            if (DescriptorWriteHandler != null)
                DescriptorWriteHandler(result, status);
        }

        public override void OnCharacteristicChanged(CyCharacteristicChangedInfo info)
        {
            if (CharacteristicChangedHandler != null)
                CharacteristicChangedHandler(info);
        }
    }

    #endregion
}
