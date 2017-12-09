/*
 * Created by SharpDevelop.
 * User: Sanjay
 * Date: 26-06-2017
 * Time: 11:42
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
using System;
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
	/// Class with program entry point.
	/// </summary>
	internal sealed class Program
	{
		
    	#region Setup Constants

        // Change the COM port name to the COM port of your CySmart dongle
        const string COM_PORT_NAME = "COM28";

        // Bluetooth address of the peer device you want to connect to
        static readonly CyBleBdAddress PEER_DEVICE_BD_ADDR = 
            new CyBleBdAddress(0x00A050D08793, CyBleBdAddressType.PUBLIC_ADDRESS);

        // CCCD attribute handle of the characteristic whose notification needs to be monitored
        const ushort CHARACTERISTIC_TO_MONITOR_CCCD_HANDLE = 0x0013;

        #endregion

               	
		/// <summary>
		/// Program entry point.
		/// </summary>
		[STAThread]
		private static void Main(string[] args)
		{
			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			Application.Run(new MainForm());
			  
		}
		
	}
}
