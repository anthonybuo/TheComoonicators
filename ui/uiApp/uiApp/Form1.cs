using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.Threading;
using System.Net;
using System.Diagnostics;
using System.Collections.Concurrent;

namespace uiApp
{
    public partial class Form1 : Form
    {
        System.IO.Ports.SerialPort port;
        Thread portThread;
        System.Windows.Forms.Timer portTimer;
        ConcurrentQueue<byte> dataReceived;
        ConcurrentQueue<byte> dataToSend;

        public Form1()
        {
            InitializeComponent();

            dataReceived = new ConcurrentQueue<byte>();
            dataToSend = new ConcurrentQueue<byte>();

            portTimer = new System.Windows.Forms.Timer();
            portTimer.Interval = 100;
            portTimer.Tick += portTimerTick;
            portTimer.Start();


        }

        private void portTimerTick(object sender, EventArgs e)
        {
            throw new NotImplementedException();
        }

        private void chart1_Click(object sender, EventArgs e)
        {

        }
        private void PortListUpdate()
        {
            portList.Items.Clear();
            portList.Items.AddRange(System.IO.Ports.SerialPort.GetPortNames());
            if (portList.Items.Count == 0)
                portList.Text = "No COM ports!";
            else
                portList.SelectedIndex = 0;
        }

        private void dataRecieved(object sender, SerialDataReceivedEventArgs e)
        {
            while (port.BytesToRead != 0)
            {
                dataReceived.Enqueue((byte)port.ReadByte());
            }
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void portList_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
    }
}
