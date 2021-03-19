﻿namespace uiApp
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea2 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend2 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series7 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Series series8 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Series series9 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Series series10 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Series series11 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Series series12 = new System.Windows.Forms.DataVisualization.Charting.Series();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.optionsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.chartOptionsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.clearGraphToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.label1 = new System.Windows.Forms.Label();
            this.portList = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.elevBox = new System.Windows.Forms.TextBox();
            this.aziBox = new System.Windows.Forms.TextBox();
            this.aziSetpointBox = new System.Windows.Forms.TextBox();
            this.elevSetpointBox = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.sendAziButton = new System.Windows.Forms.Button();
            this.sendElevButton = new System.Windows.Forms.Button();
            this.sendBothButton = new System.Windows.Forms.Button();
            this.aziElevChart = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.connectButton = new System.Windows.Forms.Button();
            this.errorStreamBox = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.packetStreamBox = new System.Windows.Forms.TextBox();
            this.packetCountLabel = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.outpacketStreamBox = new System.Windows.Forms.TextBox();
            this.outpacketCountLabel = new System.Windows.Forms.Label();
            this.speedTextBox = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.rawPacketBox = new System.Windows.Forms.TextBox();
            this.label10 = new System.Windows.Forms.Label();
            this.rawPacketButton = new System.Windows.Forms.Button();
            this.eStopButton = new System.Windows.Forms.Button();
            this.elevSTDEVBox = new System.Windows.Forms.TextBox();
            this.label11 = new System.Windows.Forms.Label();
            this.menuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.aziElevChart)).BeginInit();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.optionsToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(800, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.saveAsToolStripMenuItem,
            this.saveToolStripMenuItem,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // saveAsToolStripMenuItem
            // 
            this.saveAsToolStripMenuItem.Name = "saveAsToolStripMenuItem";
            this.saveAsToolStripMenuItem.Size = new System.Drawing.Size(114, 22);
            this.saveAsToolStripMenuItem.Text = "Save As";
            this.saveAsToolStripMenuItem.Click += new System.EventHandler(this.saveAsToolStripMenuItem_Click);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(114, 22);
            this.saveToolStripMenuItem.Text = "Save";
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(114, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            // 
            // optionsToolStripMenuItem
            // 
            this.optionsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.chartOptionsToolStripMenuItem});
            this.optionsToolStripMenuItem.Name = "optionsToolStripMenuItem";
            this.optionsToolStripMenuItem.Size = new System.Drawing.Size(61, 20);
            this.optionsToolStripMenuItem.Text = "Options";
            // 
            // chartOptionsToolStripMenuItem
            // 
            this.chartOptionsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.clearGraphToolStripMenuItem});
            this.chartOptionsToolStripMenuItem.Name = "chartOptionsToolStripMenuItem";
            this.chartOptionsToolStripMenuItem.Size = new System.Drawing.Size(148, 22);
            this.chartOptionsToolStripMenuItem.Text = "Chart Options";
            // 
            // clearGraphToolStripMenuItem
            // 
            this.clearGraphToolStripMenuItem.Name = "clearGraphToolStripMenuItem";
            this.clearGraphToolStripMenuItem.Size = new System.Drawing.Size(136, 22);
            this.clearGraphToolStripMenuItem.Text = "Clear Graph";
            this.clearGraphToolStripMenuItem.Click += new System.EventHandler(this.clearGraphToolStripMenuItem_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(20, 31);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(26, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Port";
            // 
            // portList
            // 
            this.portList.FormattingEnabled = true;
            this.portList.Location = new System.Drawing.Point(49, 27);
            this.portList.Name = "portList";
            this.portList.Size = new System.Drawing.Size(121, 21);
            this.portList.TabIndex = 2;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(50, 52);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(44, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "Azimuth";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(131, 52);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(51, 13);
            this.label3.TabIndex = 6;
            this.label3.Text = "Elevation";
            // 
            // elevBox
            // 
            this.elevBox.Location = new System.Drawing.Point(120, 68);
            this.elevBox.Name = "elevBox";
            this.elevBox.Size = new System.Drawing.Size(62, 20);
            this.elevBox.TabIndex = 7;
            this.elevBox.Text = "0";
            // 
            // aziBox
            // 
            this.aziBox.Location = new System.Drawing.Point(50, 68);
            this.aziBox.Name = "aziBox";
            this.aziBox.Size = new System.Drawing.Size(64, 20);
            this.aziBox.TabIndex = 9;
            this.aziBox.Text = "0";
            // 
            // aziSetpointBox
            // 
            this.aziSetpointBox.Location = new System.Drawing.Point(50, 95);
            this.aziSetpointBox.Name = "aziSetpointBox";
            this.aziSetpointBox.Size = new System.Drawing.Size(64, 20);
            this.aziSetpointBox.TabIndex = 10;
            this.aziSetpointBox.Text = "0";
            this.aziSetpointBox.Leave += new System.EventHandler(this.aziSetpointBox_Leave);
            // 
            // elevSetpointBox
            // 
            this.elevSetpointBox.Location = new System.Drawing.Point(120, 95);
            this.elevSetpointBox.Name = "elevSetpointBox";
            this.elevSetpointBox.Size = new System.Drawing.Size(62, 20);
            this.elevSetpointBox.TabIndex = 10;
            this.elevSetpointBox.Text = "0";
            this.elevSetpointBox.Leave += new System.EventHandler(this.elevSetpointBox_Leave);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(9, 71);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(37, 13);
            this.label4.TabIndex = 11;
            this.label4.Text = "Actual";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(0, 98);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(46, 13);
            this.label5.TabIndex = 11;
            this.label5.Text = "Setpoint";
            // 
            // sendAziButton
            // 
            this.sendAziButton.Location = new System.Drawing.Point(50, 121);
            this.sendAziButton.Name = "sendAziButton";
            this.sendAziButton.Size = new System.Drawing.Size(64, 34);
            this.sendAziButton.TabIndex = 12;
            this.sendAziButton.Text = "Send Azimuth";
            this.sendAziButton.UseVisualStyleBackColor = true;
            this.sendAziButton.Click += new System.EventHandler(this.sendAziButton_Click);
            // 
            // sendElevButton
            // 
            this.sendElevButton.Location = new System.Drawing.Point(120, 121);
            this.sendElevButton.Name = "sendElevButton";
            this.sendElevButton.Size = new System.Drawing.Size(62, 34);
            this.sendElevButton.TabIndex = 12;
            this.sendElevButton.Text = "Send Elevation";
            this.sendElevButton.UseVisualStyleBackColor = true;
            this.sendElevButton.Click += new System.EventHandler(this.sendElevButton_Click);
            // 
            // sendBothButton
            // 
            this.sendBothButton.Location = new System.Drawing.Point(50, 161);
            this.sendBothButton.Name = "sendBothButton";
            this.sendBothButton.Size = new System.Drawing.Size(132, 23);
            this.sendBothButton.TabIndex = 13;
            this.sendBothButton.Text = "Send Both";
            this.sendBothButton.UseVisualStyleBackColor = true;
            this.sendBothButton.Click += new System.EventHandler(this.sendBothButton_Click);
            // 
            // aziElevChart
            // 
            chartArea2.AxisX.Title = "Time (s)";
            chartArea2.AxisY.Title = "Position (Degrees)";
            chartArea2.AxisY2.Interval = 1D;
            chartArea2.AxisY2.Maximum = 1D;
            chartArea2.AxisY2.Minimum = 0D;
            chartArea2.Name = "ChartArea1";
            this.aziElevChart.ChartAreas.Add(chartArea2);
            legend2.Name = "Legend1";
            this.aziElevChart.Legends.Add(legend2);
            this.aziElevChart.Location = new System.Drawing.Point(12, 200);
            this.aziElevChart.Name = "aziElevChart";
            series7.ChartArea = "ChartArea1";
            series7.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            series7.Legend = "Legend1";
            series7.Name = "Elevation";
            series8.ChartArea = "ChartArea1";
            series8.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            series8.Legend = "Legend1";
            series8.Name = "Azimuth";
            series9.ChartArea = "ChartArea1";
            series9.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.StepLine;
            series9.Legend = "Legend1";
            series9.Name = "Elevation Setpoint";
            series10.ChartArea = "ChartArea1";
            series10.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.StepLine;
            series10.Legend = "Legend1";
            series10.Name = "Azimuth Setpoint";
            series11.BackImageTransparentColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(192)))), ((int)(((byte)(192)))));
            series11.ChartArea = "ChartArea1";
            series11.Legend = "Legend1";
            series11.Name = "CW Limit Switch";
            series11.YAxisType = System.Windows.Forms.DataVisualization.Charting.AxisType.Secondary;
            series12.ChartArea = "ChartArea1";
            series12.Legend = "Legend1";
            series12.Name = "CCW Limit Switch";
            series12.YAxisType = System.Windows.Forms.DataVisualization.Charting.AxisType.Secondary;
            this.aziElevChart.Series.Add(series7);
            this.aziElevChart.Series.Add(series8);
            this.aziElevChart.Series.Add(series9);
            this.aziElevChart.Series.Add(series10);
            this.aziElevChart.Series.Add(series11);
            this.aziElevChart.Series.Add(series12);
            this.aziElevChart.Size = new System.Drawing.Size(776, 238);
            this.aziElevChart.SuppressExceptions = true;
            this.aziElevChart.TabIndex = 14;
            this.aziElevChart.Text = "Chart1";
            // 
            // connectButton
            // 
            this.connectButton.Location = new System.Drawing.Point(176, 26);
            this.connectButton.Name = "connectButton";
            this.connectButton.Size = new System.Drawing.Size(75, 23);
            this.connectButton.TabIndex = 15;
            this.connectButton.Text = "Connect";
            this.connectButton.UseVisualStyleBackColor = true;
            this.connectButton.Click += new System.EventHandler(this.connectButton_Click);
            // 
            // errorStreamBox
            // 
            this.errorStreamBox.Location = new System.Drawing.Point(210, 111);
            this.errorStreamBox.Multiline = true;
            this.errorStreamBox.Name = "errorStreamBox";
            this.errorStreamBox.Size = new System.Drawing.Size(190, 83);
            this.errorStreamBox.TabIndex = 16;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(207, 95);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(65, 13);
            this.label6.TabIndex = 17;
            this.label6.Text = "Error Stream";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(403, 95);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(118, 13);
            this.label7.TabIndex = 19;
            this.label7.Text = "Packet Instream Count:";
            // 
            // packetStreamBox
            // 
            this.packetStreamBox.Location = new System.Drawing.Point(404, 111);
            this.packetStreamBox.Multiline = true;
            this.packetStreamBox.Name = "packetStreamBox";
            this.packetStreamBox.Size = new System.Drawing.Size(190, 83);
            this.packetStreamBox.TabIndex = 18;
            this.packetStreamBox.TextChanged += new System.EventHandler(this.textBox2_TextChanged);
            // 
            // packetCountLabel
            // 
            this.packetCountLabel.AutoSize = true;
            this.packetCountLabel.Location = new System.Drawing.Point(527, 95);
            this.packetCountLabel.Name = "packetCountLabel";
            this.packetCountLabel.Size = new System.Drawing.Size(13, 13);
            this.packetCountLabel.TabIndex = 20;
            this.packetCountLabel.Text = "0";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(595, 95);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(126, 13);
            this.label8.TabIndex = 22;
            this.label8.Text = "Packet Outstream Count:";
            // 
            // outpacketStreamBox
            // 
            this.outpacketStreamBox.Location = new System.Drawing.Point(598, 111);
            this.outpacketStreamBox.Multiline = true;
            this.outpacketStreamBox.Name = "outpacketStreamBox";
            this.outpacketStreamBox.Size = new System.Drawing.Size(190, 83);
            this.outpacketStreamBox.TabIndex = 21;
            // 
            // outpacketCountLabel
            // 
            this.outpacketCountLabel.AutoSize = true;
            this.outpacketCountLabel.Location = new System.Drawing.Point(727, 95);
            this.outpacketCountLabel.Name = "outpacketCountLabel";
            this.outpacketCountLabel.Size = new System.Drawing.Size(13, 13);
            this.outpacketCountLabel.TabIndex = 23;
            this.outpacketCountLabel.Text = "0";
            // 
            // speedTextBox
            // 
            this.speedTextBox.Location = new System.Drawing.Point(210, 68);
            this.speedTextBox.Name = "speedTextBox";
            this.speedTextBox.Size = new System.Drawing.Size(80, 20);
            this.speedTextBox.TabIndex = 24;
            this.speedTextBox.Text = "1000";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(207, 52);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(83, 13);
            this.label9.TabIndex = 25;
            this.label9.Text = "Speed (mdeg/s)";
            // 
            // rawPacketBox
            // 
            this.rawPacketBox.Location = new System.Drawing.Point(300, 68);
            this.rawPacketBox.Name = "rawPacketBox";
            this.rawPacketBox.Size = new System.Drawing.Size(170, 20);
            this.rawPacketBox.TabIndex = 26;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(297, 52);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(128, 13);
            this.label10.TabIndex = 27;
            this.label10.Text = "Raw Packet (FF-01-EE...)";
            // 
            // rawPacketButton
            // 
            this.rawPacketButton.Location = new System.Drawing.Point(476, 66);
            this.rawPacketButton.Name = "rawPacketButton";
            this.rawPacketButton.Size = new System.Drawing.Size(90, 23);
            this.rawPacketButton.TabIndex = 28;
            this.rawPacketButton.Text = "Send Packet";
            this.rawPacketButton.UseVisualStyleBackColor = true;
            this.rawPacketButton.Click += new System.EventHandler(this.rawPacketButton_Click);
            // 
            // eStopButton
            // 
            this.eStopButton.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(128)))), ((int)(((byte)(128)))));
            this.eStopButton.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.eStopButton.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.eStopButton.Location = new System.Drawing.Point(713, 31);
            this.eStopButton.Name = "eStopButton";
            this.eStopButton.Size = new System.Drawing.Size(75, 58);
            this.eStopButton.TabIndex = 29;
            this.eStopButton.Text = "STOP";
            this.eStopButton.UseVisualStyleBackColor = false;
            this.eStopButton.Click += new System.EventHandler(this.eStopButton_click);
            // 
            // elevSTDEVBox
            // 
            this.elevSTDEVBox.Location = new System.Drawing.Point(584, 69);
            this.elevSTDEVBox.Name = "elevSTDEVBox";
            this.elevSTDEVBox.Size = new System.Drawing.Size(100, 20);
            this.elevSTDEVBox.TabIndex = 30;
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(581, 54);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(126, 13);
            this.label11.TabIndex = 31;
            this.label11.Text = "Elevation St Dev (100 pt)";
            this.label11.Click += new System.EventHandler(this.label11_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.label11);
            this.Controls.Add(this.elevSTDEVBox);
            this.Controls.Add(this.eStopButton);
            this.Controls.Add(this.rawPacketButton);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.rawPacketBox);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.speedTextBox);
            this.Controls.Add(this.outpacketCountLabel);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.outpacketStreamBox);
            this.Controls.Add(this.packetCountLabel);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.packetStreamBox);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.errorStreamBox);
            this.Controls.Add(this.connectButton);
            this.Controls.Add(this.aziElevChart);
            this.Controls.Add(this.sendBothButton);
            this.Controls.Add(this.sendElevButton);
            this.Controls.Add(this.sendAziButton);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.elevSetpointBox);
            this.Controls.Add(this.aziSetpointBox);
            this.Controls.Add(this.aziBox);
            this.Controls.Add(this.elevBox);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.portList);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.Text = "git";
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.aziElevChart)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveAsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem optionsToolStripMenuItem;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox portList;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox elevBox;
        private System.Windows.Forms.TextBox aziBox;
        private System.Windows.Forms.TextBox aziSetpointBox;
        private System.Windows.Forms.TextBox elevSetpointBox;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Button sendAziButton;
        private System.Windows.Forms.Button sendElevButton;
        private System.Windows.Forms.Button sendBothButton;
        private System.Windows.Forms.DataVisualization.Charting.Chart aziElevChart;
        private System.Windows.Forms.Button connectButton;
        private System.Windows.Forms.TextBox errorStreamBox;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox packetStreamBox;
        private System.Windows.Forms.Label packetCountLabel;
        private System.Windows.Forms.ToolStripMenuItem chartOptionsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem clearGraphToolStripMenuItem;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox outpacketStreamBox;
        private System.Windows.Forms.Label outpacketCountLabel;
        private System.Windows.Forms.TextBox speedTextBox;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox rawPacketBox;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Button rawPacketButton;
        private System.Windows.Forms.Button eStopButton;
        private System.Windows.Forms.TextBox elevSTDEVBox;
        private System.Windows.Forms.Label label11;
    }
}

