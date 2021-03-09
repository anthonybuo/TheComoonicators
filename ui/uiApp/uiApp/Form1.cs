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
        List<packet> inPackets;
        List<packet> outPackets;
        Int32 inPacketCount = 0;
        Int32 outPacketCount = 0;

        public const int IN_PACKET_LEN = 9;
        public const int OUT_PACKET_LEN = 9;
        public const double AZI_DEG_PER_BIT = 0.04;
        public const double ELEV_DEG_PER_BIT = 0.057;

        public Form1()
        {
            InitializeComponent();

            port = new System.IO.Ports.SerialPort
            {
                BaudRate = 115200,
                DataBits = 8,
                Handshake = Handshake.None,
                StopBits = StopBits.One
            };
            port.DataReceived += dataRecieved;

            PortListUpdate();

            dataReceived = new ConcurrentQueue<byte>();
            dataToSend = new ConcurrentQueue<byte>();
            inPackets = new List<packet>();
            inPackets[0] = new packet(IN_PACKET_LEN); 
            outPackets = new List<packet>();


            portTimer = new System.Windows.Forms.Timer();
            portTimer.Interval = 100;
            portTimer.Tick += portTimerTick;
            portTimer.Start();
        }

        private void portTimerTick(object sender, EventArgs e)
        {
            if (port.IsOpen == false)
                return;

            //ConcurrentQueue<int> anotherQueue = new System.Collections.Concurrent.ConcurrentQueue<int>();
            //anotherQueue.Enqueue(5);
            //anotherQueue.Enqueue(5);
            //anotherQueue.Enqueue(5);
            //anotherQueue.Enqueue(5);
            //int tempp;
            //bool resultssts = anotherQueue.TryDequeue(out tempp);
            //resultssts = anotherQueue.TryDequeue(out tempp);
            //resultssts = anotherQueue.TryDequeue(out tempp);
            //resultssts = anotherQueue.TryDequeue(out tempp);

            inPackets[inPacketCount].unpack(dataReceived);

            if(inPackets[inPacketCount].index == inPackets[inPacketCount].PACKET_LENGTH)
            {
                aziBox.Text = inPackets[inPacketCount].azi.ToString();
                elevBox.Text = inPackets[inPacketCount].elev.ToString();

                inPacketCount++;
                inPackets[inPacketCount] = new packet(IN_PACKET_LEN);
            }
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

        private void label5_Click(object sender, EventArgs e)
        {

        }

        private void sendAziButton_Click(object sender, EventArgs e)
        {
            outPackets[outPacketCount] = new packet(OUT_PACKET_LEN);
            outPackets[outPacketCount].setAzi(double.Parse(aziSetpointBox.Text));
            outPackets[outPacketCount].pack();
        }
    }

    class packet
    {
        public byte[] data;
        public int index;
        public int PACKET_LENGTH;
        public double elev, azi;
        public packet(int packLen)
        {
            PACKET_LENGTH = packLen);
            data = new byte[PACKET_LENGTH];
            index = 0;
        }

        public void unpack(ConcurrentQueue<byte> bytes)
        {
            byte temp;
            while (bytes.TryDequeue(out temp) == true)
            {
                if (temp == 255)
                {
                    index = 0;
                    data[index] = (byte)temp;
                    index++;
                }
                else if (1 <= index && index < PACKET_LENGTH)
                {
                    data[index] = (byte)temp;
                    index++;

                    //if packet is now full (entered last byte)
                    if (index == PACKET_LENGTH)
                    {
                        index = 0;
                        parseElev();
                        parseAzi();
                        break; //break even if more to dequeue
                    }
                }
            }
        }

        public void pack(int cmd, double elev, double azi, int speed)
        {
            data[0] = 255;
            data[1] = (byte)cmd;
            

        }

        private void parseElev()
        {
            elev = (double)((int)data[1] << 8 + data[2]) * Form1.ELEV_DEG_PER_BIT;
        }

        private void parseAzi()
        {
            azi = (double)((int)data[3] << 8 + data[4]) * Form1.AZI_DEG_PER_BIT;
        }
        public void setElev(double elevIn)
        {
            elev = elevIn;
            int elevInt = (int)(elevIn / Form1.ELEV_DEG_PER_BIT);
            data[1] = (byte) (elevInt >> 8);
            data[2] = (byte)elevInt;
        }

        public void setAzi(double aziIn)
        {
            azi = aziIn;
            int aziInt = (int)(aziIn / Form1.ELEV_DEG_PER_BIT);
            data[3] = (byte)(aziInt >> 8);
            data[4] = (byte)aziInt;
        }
    }
}


