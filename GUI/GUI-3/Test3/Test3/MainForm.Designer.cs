﻿/*
 * Created by SharpDevelop.
 * User: Sanjay
 * Date: 26-06-2017
 * Time: 11:42
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
namespace Test3
{
	partial class MainForm
	{
		/// <summary>
		/// Designer variable used to keep track of non-visual components.
		/// </summary>
		private ZedGraph.ZedGraphControl z1;
		private System.ComponentModel.IContainer components = null;
		
		private System.Windows.Forms.Button COMConnect;
		private System.Windows.Forms.RichTextBox COMPortText;
		private System.Windows.Forms.Button ConnectButton;
		private System.Windows.Forms.Button Notify_Button;
		private System.Windows.Forms.TextBox NotificationText;
		private System.Windows.Forms.TextBox WriteTextBox;
		private System.Windows.Forms.Button Write_Button;
		private System.Windows.Forms.Button StartButton;
		private System.Windows.Forms.Button StopButton;
		/// <summary>
		/// Disposes resources used by the form.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing) {
				if (components != null) {
					components.Dispose();
				}
			}
			base.Dispose(disposing);
		}
		
		/// <summary>
		/// This method is required for Windows Forms designer support.
		/// Do not change the method contents inside the source code editor. The Forms designer might
		/// not be able to load this method if it was changed manually.
		/// </summary>
		private void InitializeComponent()
		{
			// 
			// MainForm
			// 
			/*this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Text = "Test3";
			this.Name = "MainForm";*/
			this.z1 = new ZedGraph.ZedGraphControl();
			this.SuspendLayout();
			
			// 
			// zedGraphControl1
			// 
			this.z1.Location = new System.Drawing.Point(0, 0);
			this.z1.Name = "zedGraphControl1";
			this.z1.Size = new System.Drawing.Size(1000, 500);
			this.z1.TabIndex = 0;
						
			this.COMConnect = new System.Windows.Forms.Button();
			this.COMPortText = new System.Windows.Forms.RichTextBox();
			this.ConnectButton = new System.Windows.Forms.Button();
			this.Notify_Button = new System.Windows.Forms.Button(); 
			this.NotificationText = new System.Windows.Forms.TextBox();
			this.Write_Button = new System.Windows.Forms.Button();
			this.WriteTextBox = new System.Windows.Forms.TextBox();
			this.StartButton = new System.Windows.Forms.Button();
			this.StopButton = new System.Windows.Forms.Button();
			
			// 
            // COMPortText
            // 
            this.COMPortText.Location = new System.Drawing.Point(101, 550);
            this.COMPortText.Name = "COMPortText";
            this.COMPortText.Size = new System.Drawing.Size(66, 24);
            this.COMPortText.TabIndex = 0;
            this.COMPortText.Text = "COM28";
            this.COMPortText.TextChanged += new System.EventHandler(this.COMPortText_TextChanged);
            
			// 
            // COMConnect
            // 
            this.COMConnect.Location = new System.Drawing.Point(300, 550);
            this.COMConnect.Name = "COMConnect";
            this.COMConnect.Size = new System.Drawing.Size(101, 23);
            this.COMConnect.TabIndex = 1;
            this.COMConnect.Text = "Connect to COM";
            this.COMConnect.UseVisualStyleBackColor = true;
            this.COMConnect.Click += new System.EventHandler(this.COM_Connect_Click);
            
            // 
            // ConnectButton
            // 
            this.ConnectButton.Location = new System.Drawing.Point(450, 550);
            this.ConnectButton.Name = "ConnectButton";
            this.ConnectButton.Size = new System.Drawing.Size(75, 23);
            this.ConnectButton.TabIndex = 5;
            this.ConnectButton.Text = "Connect";
            this.ConnectButton.UseVisualStyleBackColor = true;
            this.ConnectButton.Click += new System.EventHandler(this.ConnectButton_Click);
            
            // 
            // Notify_Button
            // 
            this.Notify_Button.Enabled = false;
            this.Notify_Button.Location = new System.Drawing.Point(560, 550);
            this.Notify_Button.Name = "Notify_Button";
            this.Notify_Button.Size = new System.Drawing.Size(125, 23);
            this.Notify_Button.TabIndex = 6;
            this.Notify_Button.Text = "Enable Notification";
            this.Notify_Button.UseVisualStyleBackColor = true;
            this.Notify_Button.Click += new System.EventHandler(this.Notify_Button_Click);
            
            // 
            // NotificationText
            // 
            this.NotificationText.Enabled = true;
            this.NotificationText.Location = new System.Drawing.Point(700, 550);
            this.NotificationText.Name = "NotificationText";
            this.NotificationText.Size = new System.Drawing.Size(100, 20);
            this.NotificationText.TabIndex = 7;
            
            // 
            // WriteTextBox
            // 
            this.WriteTextBox.Enabled = false;
            this.WriteTextBox.Location = new System.Drawing.Point(450, 650);
            this.WriteTextBox.Name = "WriteTextBox";
            this.WriteTextBox.Size = new System.Drawing.Size(100, 20);
            this.WriteTextBox.TabIndex = 9;

            // 
            // Write_Button
            // 
            this.Write_Button.Enabled = false;
            this.Write_Button.Location = new System.Drawing.Point(560, 650);
            this.Write_Button.Name = "Write_Button";
            this.Write_Button.Size = new System.Drawing.Size(75, 23);
            this.Write_Button.TabIndex = 12;
            this.Write_Button.Text = "Write";
            this.Write_Button.UseVisualStyleBackColor = true;
            this.Write_Button.Click += new System.EventHandler(this.Write_Button_Click);          
            
            // 
            // StartButton
            // 
            this.StartButton.Location = new System.Drawing.Point(350, 600);
            this.StartButton.Name = "StartButton";
            this.StartButton.Size = new System.Drawing.Size(75, 23);
            this.StartButton.TabIndex = 5;
            this.StartButton.Text = "Start";
            this.StartButton.UseVisualStyleBackColor = true;
            this.StartButton.Click += new System.EventHandler(this.StartButton_Click);
            
            // 
            // StopButton
            // 
            this.StopButton.Location = new System.Drawing.Point(550, 600);
            this.StopButton.Name = "StopButton";
            this.StopButton.Size = new System.Drawing.Size(75, 23);
            this.StopButton.TabIndex = 5;
            this.StopButton.Text = "Stop";
            this.StopButton.UseVisualStyleBackColor = true;
            this.StopButton.Click += new System.EventHandler(this.StopButton_Click);
            
            // 
			// MainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(784, 597);
			this.Controls.Add(this.z1);			
			this.Controls.Add(this.COMConnect);
			this.Controls.Add(this.COMPortText);
			this.Controls.Add(this.ConnectButton);
			this.Controls.Add(this.Notify_Button);
			this.Controls.Add(this.NotificationText);		
			this.Controls.Add(this.Write_Button);
			this.Controls.Add(this.WriteTextBox);
			this.Controls.Add(this.StartButton);
			this.Controls.Add(this.StopButton);
							
			this.Name = "MainForm";
			this.Text = "Test3";
			this.ResumeLayout(false);
			this.Load += new System.EventHandler(this.MainForm_Load);			
			this.PerformLayout();

		}
	}
}
