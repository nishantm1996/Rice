/*
 * Created by SharpDevelop.
 * User: Sanjay
 * Date: 26-06-2017
 * Time: 11:42
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
using System;
using System.Linq;
using System.Collections;
using System.ComponentModel;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using System.Data;
using ZedGraph;

using System.Text;
using System.IO;

using CySmart.DongleCommunicator.API;
using CySmart.Common.Base.Compatibility.Net20;
using System.Threading;

namespace Test3
{
	/// <summary>
	/// Description of MainForm.
	/// </summary>
		
			
	#region MainForm

    /// <summary>
    /// Simple class which logs all received notifications to Console
    /// </summary>
    public partial class MainForm : Form, IDisposable
    {
        //static int[] y;
   		double[] x = new double[1000];
		double[] y = new double[1000];   	
		
		
    	#region Setup Constants

        // Change the COM port name to the COM port of your CySmart dongle
        string COM_PORT_NAME;

        // Bluetooth address of the peer device you want to connect to
        static readonly CyBleBdAddress PEER_DEVICE_BD_ADDR = 
            new CyBleBdAddress(0x00A050D06709, CyBleBdAddressType.PUBLIC_ADDRESS);

        // CCCD attribute handle of the characteristic whose notification needs to be monitored
        const ushort CHARACTERISTIC_TO_MONITOR_CCCD_HANDLE = 0x0013;
        const ushort LED_HANDLE = 0x0016 ;
        #endregion

        #region consts

        // Value to enable notification
        const ushort ENABLE_NOTIFICATION = 0x0001;

        // Value to disable notification
        const ushort DISABLE_NOTIFICATION = 0x0000;

        #endregion

        #region members
        CyDongleInfo MyDongle;
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
        
        bool NotificationEnabled = false;

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

        public void COMPortText_TextChanged(object sender, EventArgs e)
        {

        }
	
    public void COM_Connect_Click(object sender, System.EventArgs e)
        {
    	    COM_PORT_NAME = COMPortText.Text;
    	    MyDongle = new CyDongleInfo(COM_PORT_NAME, CyDongleInfo.CySmartDongleType.CY5670);
            
            CySmartDongleMgr dongleMgr = CySmartDongleMgr.GetInstance();
            CyApiErr err = dongleMgr.TryGetCySmartDongleCommunicator(MyDongle, out m_communicator);
            if (err.IsOK)
            {
                MessageBox.Show("Dongle Connected");
                m_bleMgrCb = new BleMgrCb();
                err = BleMgr.RegisterBleMgrCallback(m_bleMgrCb);
            }
            else return;         
            
        }
    int count,cc,connb;
    public void ConnectButton_Click(object sender, System.EventArgs e)
    {
    	    AutoResetEvent sync = new AutoResetEvent(false);
            CyApiErr erro = CyApiErr.OK;

            // Set the connection handler
            m_bleMgrCb.ConnectionHandler = (result, status) =>
            	
            {
                if (status == CyStatus.BLE_STATUS_OK)
                {
                    MessageBox.Show("Connected!");
                	m_peerDevice = result.Device;
                    
                    // Register the device callback
                    m_deviceCb = new DeviceCb();
                    m_peerDevice.RegisterCallback(m_deviceCb);

                    // Register the GATT client callback
                    m_gattClientCb = new GattClientCb();
                    m_peerDevice.GattClient.RegisterCallback(m_gattClientCb);
                    m_gattClientCb.CharacteristicChangedHandler = (info) =>
            {
                    	count = count + 1; 
                    	    if (NotificationText.InvokeRequired)
                            {
                                Invoke((MethodInvoker)(() =>
                                {
                                    NotificationText.Text = count.ToString();
                                }));

                            }
                            else
                            {
                                NotificationText.Text = count.ToString();
                            }
                            // Received notifications follow the format BB:AA, where AA is the first 8-bit hex number, and BB the second one 
                            ((IPointListEdit)z1.GraphPane.CurveList[0].Points).Add(count,Convert.ToInt32(GetPrintableValue(info.Value).Substring(3,2)+GetPrintableValue(info.Value).Substring(0,2),16));
							// Log notifications in text file only if start button has been clicked, and stop logging if stop button has been clicked
                            using (System.IO.StreamWriter file = new System.IO.StreamWriter("log"+cc.ToString()+".txt", true))
        					{
                            	if (connb == 1)
                            		file.WriteLine(count + "," + Convert.ToInt32(GetPrintableValue(info.Value).Substring(3,2)+GetPrintableValue(info.Value).Substring(0,2),16));
                            	else
                            		file.Close();
        					}   
				    z1.AxisChange();
				    z1.Invalidate();				    
            
            };                 
             
                }
                else
                {
                    erro = new CyApiErr("Failed to connect to the peer device. Reason: " + status.ToString());
                }

                sync.Set();
            };            

            // Initiate connection request to the peer BLE device
            erro = BleMgr.Connect(new CyConnectInfo(PEER_DEVICE_BD_ADDR, new CyBleConnectionSettings()));
               if (erro.IsOK)
                {
                    sync.WaitOne();
                    Notify_Button.Enabled = true;
                    Write_Button.Enabled = true;
                    WriteTextBox.Enabled = true;
                    Notify_Button.Text = "Enable Notification";                    
                }      
            

            
    }
    // Update flag for logging files and count the number of times the start button has been clicked
    public void StartButton_Click(object sender, System.EventArgs e)
    {
    	connb = 1; cc = cc + 1;
    }
    // Update flag for logging files 
    public void StopButton_Click(object sender, System.EventArgs e)
    {
    	connb = 0;
    }
    
    public void Notify_Button_Click(object sender, System.EventArgs e)
    {
    	    AutoResetEvent synco = new AutoResetEvent(false);
            CyApiErr err = CyApiErr.OK;

            // Setup the descriptor write handler
            m_gattClientCb.DescriptorWriteHandler = (CyConnectResult, status) =>
            {
                if (status != CyStatus.BLE_STATUS_OK)
                    err = new CyApiErr("Failed to start/stop notification monitoring. Reason: " + status.ToString());

                synco.Set();
            };

            // Initiate write descriptor request to the CCCD
            err = GattClient.WriteDescriptor(new CyGattWriteInfo(CHARACTERISTIC_TO_MONITOR_CCCD_HANDLE, BitConverter.GetBytes(ENABLE_NOTIFICATION)));
            if (!(NotificationEnabled))
            {
                // Initiate write descriptor request to the CCCD
                err = GattClient.WriteDescriptor(new CyGattWriteInfo(CHARACTERISTIC_TO_MONITOR_CCCD_HANDLE, BitConverter.GetBytes(ENABLE_NOTIFICATION)));

                if (err.IsOK)
                {
                    synco.WaitOne();
                    NotificationEnabled = true;
                    
                    Notify_Button.Text = "Disable Notification";

                }
            }
            else
            {
                err = GattClient.WriteDescriptor(new CyGattWriteInfo(CHARACTERISTIC_TO_MONITOR_CCCD_HANDLE, BitConverter.GetBytes(DISABLE_NOTIFICATION)));

                if (err.IsOK)
                {
                    synco.WaitOne();
                    NotificationEnabled = false;
                    Notify_Button.Text = "Enable Notification";
                }
            }
    }
    
    private void Write_Button_Click(object sender, EventArgs e)
        {
            AutoResetEvent sync = new AutoResetEvent(false);
            CyApiErr err = CyApiErr.OK;

            // Setup the descriptor write handler
            m_gattClientCb.CharacteristicWriteHandler = (CyConnectResult, status) =>
            {
                if (status != CyStatus.BLE_STATUS_OK)
                    err = new CyApiErr("Failed to Write: Reason: " + status);
                else               	
                sync.Set();
            };

           
                // Initiate write descriptor request to the CCCD
            err = m_peerDevice.GattClient.WriteCharacteristic(new CyGattWriteInfo(LED_HANDLE, BitConverter.GetBytes(int.Parse(WriteTextBox.Text))));
            if (err.IsOK)
                {
                    sync.WaitOne();
                    MessageBox.Show ("Value written!");
                }
                else 
                {
                    MessageBox.Show ("Write Failed: Reason :"+err.ToString());
                }
        }
    
        #region ctor

        public MainForm()
        {
            InitializeComponent();
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
                    RegisterMainForm();
                    
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
        public CyApiErr StartStopLogging(params byte[] data)
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
        public void RegisterMainForm()
        {
            // Setup the characteristic changed handler
            m_gattClientCb.CharacteristicChangedHandler = (info) =>
            {
                // Log all received notifications
                    int loc;
            		            		
            		string str = GetPrintableValue(info.Value);
            		str = str.Substring(0,2);            		
            		int intTemp = Convert.ToInt32(str, 16); 
				    loc = intTemp;		
				    NotificationText.Text = str;
				    y[count] = loc;
				    count ++;
				    z1.GraphPane.CurveList.Clear();
				    z1.AxisChange();
				    z1.Invalidate();
				    z1.GraphPane.AddCurve("", x, y, Color.Red, SymbolType.Default);
            
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

        public CySmart.Common.Base.Compatibility.Net20.Action<CyGattReadResult, CyStatus> ReadHandler { get; set; }

        public CySmart.Common.Base.Compatibility.Net20.Action<CyGattWriteResult, CyStatus> CharacteristicWriteHandler { get; set; }



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
        
        public override void OnCharacteristicRead(CyGattReadResult result, CyStatus status)
        {
            if (ReadHandler != null)
                ReadHandler(result, status);

            
        }

        public override void OnCharacteristicWrite(CyGattWriteResult result, CyStatus status)
        {
            if (CharacteristicWriteHandler != null)
                CharacteristicWriteHandler(result, status);
        }
    }

    #endregion
    #endregion
    
	
    public void MainForm_Load( object sender, System.EventArgs e )
		{
			  
			z1.IsShowPointValues = true;
			z1.IsZoomOnMouseCenter = true;   
            z1.IsEnableHZoom = true;
			int i;
			// Initial curve to which points are serially added 
			for (i=0;i<1000;i++)
			{
		    	x[i] = i; y[i] = 0;
	    	}
			z1.GraphPane.AddCurve("", x, y, Color.Red, SymbolType.None);	
			z1.AxisChange();
			z1.Invalidate();			
            
		}    
    
    }
}