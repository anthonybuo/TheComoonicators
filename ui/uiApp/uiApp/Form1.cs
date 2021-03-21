using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Drawing;
using System.IO.Ports;
using System.Windows.Forms;

namespace uiApp
{
    public partial class Form1 : Form
    {
        System.IO.Ports.SerialPort port;
        System.Windows.Forms.Timer uiTimer;
        ConcurrentQueue<byte> dataReceived;
        ConcurrentQueue<byte> dataToSend;
        List<InPacket> inPackets;
        List<OutPacket> outPackets;
        Int32 inPacketCount = 0;
        Int32 outPacketCount = 0;

        public const int IN_PACKET_LEN = 9;
        public const int OUT_PACKET_LEN = 9;
        public const double AZI_DEG_PER_BIT = 0.04;
        public const double ELEV_DEG_PER_BIT = 0.057;

        double chartTime = 0;

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
            inPackets = new List<InPacket>();
            inPackets.Add(new InPacket(IN_PACKET_LEN)); 
            outPackets = new List<OutPacket>();

            aziElevChart.Series[4].Color = Color.FromArgb(64, 60, 60, 60);
            aziElevChart.Series[4].Color = Color.FromArgb(64, 60, 0, 60);

            uiTimer = new System.Windows.Forms.Timer();
            uiTimer.Interval = 100;
            uiTimer.Tick += uiTimerTick;
            uiTimer.Start();
        }

        private void uiTimerTick(object sender, EventArgs e)
        {
            if (port.IsOpen == false)
                return;

            int filledPacketBool;
            do
            {
                filledPacketBool = inPackets[inPacketCount].unpack(dataReceived);
                if(filledPacketBool == 1)
                {
                    aziBox.Text = inPackets[inPacketCount].azi.ToString();
                    elevBox.Text = inPackets[inPacketCount].elev.ToString();
                    updateChart(inPackets[inPacketCount]);
                    updatePacketStream(inPackets[inPacketCount]);
                    updateErrorStream(inPackets[inPacketCount]);
                    inPacketCount++;
                    inPackets.Add(new InPacket(IN_PACKET_LEN));
                }
            }
            while(filledPacketBool == 1);
            packetCountLabel.Text = inPacketCount.ToString();
            int breakpoint = 1;
        }

        private void updateErrorStream(InPacket pkt)
        {
            errorStreamBox.Clear();
            for (int i = 0; i < InPacket.errorCodes.Length; i++)
            {
                if (pkt.errors[i])
                {
                    errorStreamBox.AppendText(InPacket.errorCodes[i]);
                }
            }
        }

        private void updateChart(InPacket pkt)
        {
            chartTime += 1; // / InPacket.freqB2D(pkt.data[5]);
            //TODO change this back to frequency calc once implemented
            aziElevChart.Series[0].Points.AddXY(chartTime, pkt.elev);
            aziElevChart.Series[1].Points.AddXY(chartTime, pkt.azi);
            if(outPackets.Count > 0)
            { 
                aziElevChart.Series[2].Points.AddXY(chartTime, outPackets[outPackets.Count-1].elev);
                aziElevChart.Series[3].Points.AddXY(chartTime, outPackets[outPackets.Count - 1].azi);
            }

            if (pkt.switches.cwAzi)
                aziElevChart.Series[4].Points.AddXY(chartTime, 0.25);

            if (pkt.switches.ccwAzi)
                aziElevChart.Series[5].Points.AddXY(chartTime, 0.25);

            AddBollingerBands(ref inPackets, 100, 1);

            elevSTDEVBox.Text = inPackets[inPacketCount].stdev.ToString();

            
        }

        private void updatePacketStream(InPacket pkt)
        {
            packetStreamBox.AppendText(BitConverter.ToString(pkt.data) + Environment.NewLine);
        }

        private void updateOutPacketStream(OutPacket pkt)
        {
            outpacketStreamBox.AppendText(BitConverter.ToString(pkt.data) + Environment.NewLine);
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
            if(!port.IsOpen)
            {
                return;
            }
            while (port.BytesToRead != 0)
            {
                dataReceived.Enqueue((byte)port.ReadByte());
            }
        }

        private void sendAziButton_Click(object sender, EventArgs e)
        {
            baselineSendButton(OutPacket.GOTO_AZI_CMD);
        }

        private void sendElevButton_Click(object sender, EventArgs e)
        {
            baselineSendButton(OutPacket.GOTO_ELEV_CMD);
        }

        private void sendBothButton_Click(object sender, EventArgs e)
        {
            baselineSendButton(OutPacket.GOTO_CMD);
        }

        private void baselineSendButton(int cmd)
        {
            if(!port.IsOpen)
            {
                MessageBox.Show("Port is closed");
                return;
            }
            outPackets.Add(new OutPacket(OUT_PACKET_LEN));
            double azi, elev;
            azi = double.Parse(aziSetpointBox.Text);
            elev = double.Parse(elevSetpointBox.Text);
            short speed = short.Parse(speedTextBox.Text);
            outPackets[outPacketCount].pack(cmd, elev, azi, speed);

            port.Write(outPackets[outPacketCount].data, 0, outPackets[outPacketCount].PACKET_LENGTH);

            updateOutPacketStream(outPackets[outPacketCount]);

            outPacketCount++;
        }

        private void connectButton_Click(object sender, EventArgs e)
        {
            if(port.IsOpen)
            {
                port.Close();
                connectButton.Text = "Connect";
            }
            else
            {
                port.PortName = portList.Text;
                port.Open();
                connectButton.Text = "Disconnect";
            }
        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {

        }

        private void aziSetpointBox_TextChanged(object sender, EventArgs e)
        {

        }

        private void aziSetpointBox_Leave(object sender, EventArgs e)
        {
            try
            {
                double.Parse(aziSetpointBox.Text);
            }
            catch (System.FormatException)
            {
                aziSetpointBox.Text = "0";
                MessageBox.Show("Input must be numerical");
            }
        }

        private void elevSetpointBox_Leave(object sender, EventArgs e)
        {
            try
            {
                double.Parse(elevSetpointBox.Text);
            }
            catch (System.FormatException)
            {
                aziSetpointBox.Text = "0";
                MessageBox.Show("Input must be numerical");
            }
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //SaveFileDialog saveFileDialog1 = new SaveFileDialog();
            //saveFileDialog1.Title = "Save data to file";
            //saveFileDialog1.ShowDialog();

            //// If the file name is not an empty string open it for saving.
            //if (saveFileDialog1.FileName = "")
            //{
            //    return;
            //}

            //System.IO.StreamWriter file = new System.IO.StreamWriter(saveFileDialog1.FileName);
        }

        private void clearGraphToolStripMenuItem_Click(object sender, EventArgs e)
        {
            for(int i = 0; i < aziElevChart.Series.Count; i++)
            {
                aziElevChart.Series[i].Points.Clear();
            }
        }

        public static byte[] StringToByteArrayFastest(string hex)
        // from https://stackoverflow.com/questions/321370/how-can-i-convert-a-hex-string-to-a-byte-array
        {
            if (hex.Length % 2 == 1)
                throw new Exception("The binary key cannot have an odd number of digits");

            byte[] arr = new byte[hex.Length >> 1];

            for (int i = 0; i < hex.Length >> 1; ++i)
            {
                arr[i] = (byte)((GetHexVal(hex[i << 1]) << 4) + (GetHexVal(hex[(i << 1) + 1])));
            }

            return arr;
        }

        public static int GetHexVal(char hex)
        // from https://stackoverflow.com/questions/321370/how-can-i-convert-a-hex-string-to-a-byte-array
        {
            int val = (int)hex;
            //For uppercase A-F letters:
            //return val - (val < 58 ? 48 : 55);
            //For lowercase a-f letters:
            //return val - (val < 58 ? 48 : 87);
            //Or the two combined, but a bit slower:
            return val - (val < 58 ? 48 : (val < 97 ? 55 : 87));
        }

        private void rawPacketButton_Click(object sender, EventArgs e)
        {
            if(!port.IsOpen)
            {
                MessageBox.Show("Port is closed");
                return;
            }
            string txt = rawPacketBox.Text;
            // FF-CC-EH-EL-AH-AL-SH-SL-SP
            if (txt.Length != 26)
            {
                MessageBox.Show("Incorrect packet format");
                return;
            }
            txt = txt.Replace("-", "");
            txt = txt.ToLower();

            byte[] data = StringToByteArrayFastest(txt);

            outPackets.Add(new OutPacket(OUT_PACKET_LEN));
            outPackets[outPacketCount].data = data;

            port.Write(data, 0, outPackets[outPacketCount].PACKET_LENGTH);
            updateOutPacketStream(outPackets[outPacketCount]);
            outPacketCount++;
        }

        private void eStopButton_click(object sender, EventArgs e)
        {
            byte[] data = { 255, 0x08, 0, 0, 0, 0, 0, 0, 0 };

            outPackets.Add(new OutPacket(OUT_PACKET_LEN));
            outPackets[outPacketCount].data = data;

            port.Write(data, 0, data.Length);
            updateOutPacketStream(outPackets[outPacketCount]);
            outPacketCount++;
        }

        private void setAzimuthButton_Click(object sender, EventArgs e)
        {
            if(!port.IsOpen)
            {
                MessageBox.Show("Port is closed");
                return;
            }
            outPackets.Add(new OutPacket(OUT_PACKET_LEN));
            double azimuth_setpoint = double.Parse(setAzimuthBox.Text);
            outPackets[outPacketCount].pack(0x09, 0, azimuth_setpoint, 0);

            port.Write(outPackets[outPacketCount].data, 0, outPackets[outPacketCount].PACKET_LENGTH);
            updateOutPacketStream(outPackets[outPacketCount]);
            outPacketCount++;
        }

        public static void AddBollingerBands(ref List<InPacket> packetList, int period, int factor)
        {
            double total_average = 0;
            double total_squares = 0;

            for (int i = 0; i < packetList.Count; i++)
            {
                total_average += packetList[i].elev;
                total_squares += Math.Pow(packetList[i].elev, 2);

                if (i >= period - 1)
                {
                    double total_bollinger = 0;
                    double average = total_average / period;

                    double stdev = Math.Sqrt((total_squares - Math.Pow(total_average, 2) / period) / period);
                    packetList[i].average = average;
                    packetList[i].stdev = stdev;


                    total_average -= packetList[i - period + 1].elev;
                    total_squares -= Math.Pow(packetList[i - period + 1].elev, 2);
                }
            }
        }

        private void label11_Click(object sender, EventArgs e)
        {

        }
    }

    public class Packet
    {
        public byte[] data;
        public int index;
        public int PACKET_LENGTH;
        public double elev, azi;
        public Packet(int packLen)
        {
            PACKET_LENGTH = packLen;
            data = new byte[PACKET_LENGTH];
            index = 0;
        }
        internal double elevB2D(byte[] elevByte)
        {
            double elevDouble = (double)((short)((short)elevByte[0] << 8) + elevByte[1]) * Form1.ELEV_DEG_PER_BIT;
            return elevDouble;
        }

        internal double aziB2D(byte[] aziByte)
        {
            double aziDouble = (double)((short)((short)aziByte[0] << 8) + aziByte[1]) * Form1.AZI_DEG_PER_BIT;
            return aziDouble;
        }

        internal byte[] elevD2B(double elevIn)
        {
            int elevInt = (int)(elevIn / Form1.ELEV_DEG_PER_BIT);
            byte[] elevByte = new byte[2];
            elevByte[0] = (byte)(elevInt >> 8);
            elevByte[1] = (byte)elevInt;
            return elevByte;
        }
        internal byte[] aziD2B(double aziIn)
        {
            int aziInt = (int)(aziIn / Form1.AZI_DEG_PER_BIT);
            byte[] aziByte = new byte[2];
            aziByte[0] = (byte)(aziInt >> 8);
            aziByte[1] = (byte)aziInt;
            return aziByte;
        }
    }

    public class InPacket : Packet
    { 
        public struct limitSwitchStruct
        {
            public bool cwAzi, ccwAzi, vertElev, horiElev;
        }

        public limitSwitchStruct switches;

        public bool[] errors;
        public static string[] errorCodes = {
            "Elevation command out of bounds\r\n",
            "Azimuth command out of bounds\r\n",
            "Speed command out of bounds\r\n",
            "Accelerometer reading unrealistic\r\n",
            "Accelerometer not communicating\r\n" };

        public double stdev { get; internal set; }

        public double average { get; internal set; }

        public InPacket(int packLen) : base(packLen)
        {
            errors = new bool[errorCodes.Length];
        }
        public int unpack(ConcurrentQueue<byte> bytes)
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
                        byte[] temp2 = { data[1], data[2] };
                        index = 0;
                        elev = base.elevB2D(temp2);
                        temp2[0] = data[3];
                        temp2[1] = data[4];
                        azi = base.aziB2D(temp2);

                        switches.cwAzi = (data[6] & 0b1) != 0;
                        switches.ccwAzi = (data[6] & 0b10) != 0;
                        switches.vertElev = (data[6] & 0b100) != 0;
                        switches.horiElev = (data[6] & 0b1000) != 0;

                        for (int i = 0; i < errorCodes.Length; i++)
                        {
                            errors[i] = (data[7] & (int)Math.Pow(2, i)) != 0;
                        }

                        return 1; //sucessfully filled a packet
                    }
                }
            }
            return 0; //finished dequeuing, but didn't fill the packet
        }

        public static double freqB2D(byte freqByte)
        {
            double freq = (double)((0.5 * (freqByte & 0b10000) + (byte)(freqByte & 0b1111)) * Math.Pow(10, (freqByte & 0b11100000)));
            return freq;
        }
    }

    class OutPacket : Packet
    {
        public static int GOTO_CMD = 0x1;
        public static int GOTO_AZI_CMD = 0x07;
        public static int GOTO_ELEV_CMD = 0x06;
        public static int DEFAULT_SPEED = 1000;
        public OutPacket(int packLen) : base(packLen)
        {

        }

        public byte[] pack(int cmd, double elev, double azi, int speed)
        {
            base.azi = azi;
            base.elev = elev;
            data[0] = 255;
            data[1] = (byte)cmd;
            byte[] tempByte = base.elevD2B(elev);
            data[2] = tempByte[0];
            data[3] = tempByte[1];
            tempByte = base.aziD2B(azi);
            data[4] = tempByte[0];
            data[5] = tempByte[1];
            data[6] = (byte)(speed >> 8);
            data[7] = (byte)speed;

            data[8] = 0;
            for(int i = 1; i < PACKET_LENGTH; i++)
            {
                if (data[i] == 255)
                {
                    data[i] = 254;
                    data[8] |= (byte) (1 << i);
                }
            }

            return data;
        }
    }
}


