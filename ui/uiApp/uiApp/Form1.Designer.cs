namespace uiApp
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
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend1 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Series series2 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Series series3 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Series series4 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Series series5 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Series series6 = new System.Windows.Forms.DataVisualization.Charting.Series();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.optionsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
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
            this.optionsToolStripMenuItem.Name = "optionsToolStripMenuItem";
            this.optionsToolStripMenuItem.Size = new System.Drawing.Size(61, 20);
            this.optionsToolStripMenuItem.Text = "Options";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(17, 31);
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
            this.portList.SelectedIndexChanged += new System.EventHandler(this.portList_SelectedIndexChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(62, 61);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(44, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "Azimuth";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(143, 61);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(51, 13);
            this.label3.TabIndex = 6;
            this.label3.Text = "Elevation";
            // 
            // elevBox
            // 
            this.elevBox.Location = new System.Drawing.Point(132, 77);
            this.elevBox.Name = "elevBox";
            this.elevBox.Size = new System.Drawing.Size(62, 20);
            this.elevBox.TabIndex = 7;
            // 
            // aziBox
            // 
            this.aziBox.Location = new System.Drawing.Point(62, 77);
            this.aziBox.Name = "aziBox";
            this.aziBox.Size = new System.Drawing.Size(64, 20);
            this.aziBox.TabIndex = 9;
            // 
            // aziSetpointBox
            // 
            this.aziSetpointBox.Location = new System.Drawing.Point(62, 104);
            this.aziSetpointBox.Name = "aziSetpointBox";
            this.aziSetpointBox.Size = new System.Drawing.Size(64, 20);
            this.aziSetpointBox.TabIndex = 10;
            // 
            // elevSetpointBox
            // 
            this.elevSetpointBox.Location = new System.Drawing.Point(132, 104);
            this.elevSetpointBox.Name = "elevSetpointBox";
            this.elevSetpointBox.Size = new System.Drawing.Size(62, 20);
            this.elevSetpointBox.TabIndex = 10;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(19, 80);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(37, 13);
            this.label4.TabIndex = 11;
            this.label4.Text = "Actual";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(10, 107);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(46, 13);
            this.label5.TabIndex = 11;
            this.label5.Text = "Setpoint";
            // 
            // sendAziButton
            // 
            this.sendAziButton.Location = new System.Drawing.Point(62, 130);
            this.sendAziButton.Name = "sendAziButton";
            this.sendAziButton.Size = new System.Drawing.Size(64, 34);
            this.sendAziButton.TabIndex = 12;
            this.sendAziButton.Text = "Send Azimuth";
            this.sendAziButton.UseVisualStyleBackColor = true;
            this.sendAziButton.Click += new System.EventHandler(this.sendAziButton_Click);
            // 
            // sendElevButton
            // 
            this.sendElevButton.Location = new System.Drawing.Point(132, 130);
            this.sendElevButton.Name = "sendElevButton";
            this.sendElevButton.Size = new System.Drawing.Size(62, 34);
            this.sendElevButton.TabIndex = 12;
            this.sendElevButton.Text = "Send Elevation";
            this.sendElevButton.UseVisualStyleBackColor = true;
            // 
            // sendBothButton
            // 
            this.sendBothButton.Location = new System.Drawing.Point(62, 171);
            this.sendBothButton.Name = "sendBothButton";
            this.sendBothButton.Size = new System.Drawing.Size(132, 23);
            this.sendBothButton.TabIndex = 13;
            this.sendBothButton.Text = "Send Both";
            this.sendBothButton.UseVisualStyleBackColor = true;
            // 
            // aziElevChart
            // 
            chartArea1.Name = "ChartArea1";
            this.aziElevChart.ChartAreas.Add(chartArea1);
            legend1.Name = "Legend1";
            this.aziElevChart.Legends.Add(legend1);
            this.aziElevChart.Location = new System.Drawing.Point(12, 200);
            this.aziElevChart.Name = "aziElevChart";
            series1.ChartArea = "ChartArea1";
            series1.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            series1.Legend = "Legend1";
            series1.Name = "Azimuth";
            series2.ChartArea = "ChartArea1";
            series2.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            series2.Legend = "Legend1";
            series2.Name = "Elevation";
            series3.ChartArea = "ChartArea1";
            series3.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.StepLine;
            series3.Legend = "Legend1";
            series3.Name = "Azimuth Setpoint";
            series4.ChartArea = "ChartArea1";
            series4.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.StepLine;
            series4.Legend = "Legend1";
            series4.Name = "Elevation Setpoint";
            series5.ChartArea = "ChartArea1";
            series5.Legend = "Legend1";
            series5.Name = "CW Limit Switch";
            series6.ChartArea = "ChartArea1";
            series6.Legend = "Legend1";
            series6.Name = "CCW Limit Switch";
            this.aziElevChart.Series.Add(series1);
            this.aziElevChart.Series.Add(series2);
            this.aziElevChart.Series.Add(series3);
            this.aziElevChart.Series.Add(series4);
            this.aziElevChart.Series.Add(series5);
            this.aziElevChart.Series.Add(series6);
            this.aziElevChart.Size = new System.Drawing.Size(776, 238);
            this.aziElevChart.TabIndex = 14;
            this.aziElevChart.Text = "Chart1";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.ClientSize = new System.Drawing.Size(800, 450);
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
            this.Text = "Form1";
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
    }
}

