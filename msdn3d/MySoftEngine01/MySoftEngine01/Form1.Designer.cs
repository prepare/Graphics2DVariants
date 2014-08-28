namespace SoftEngine
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
            this.components = new System.ComponentModel.Container();
            this.frontBuffer = new System.Windows.Forms.PictureBox();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.frontBuffer)).BeginInit();
            this.SuspendLayout();
            // 
            // frontBuffer
            // 
            this.frontBuffer.BackColor = System.Drawing.Color.Black;
            this.frontBuffer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.frontBuffer.Location = new System.Drawing.Point(0, 0);
            this.frontBuffer.Name = "frontBuffer";
            this.frontBuffer.Size = new System.Drawing.Size(914, 567);
            this.frontBuffer.TabIndex = 0;
            this.frontBuffer.TabStop = false;
            // 
            // timer1
            // 
            this.timer1.Interval = 500;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(914, 567);
            this.Controls.Add(this.frontBuffer);
            this.Name = "Form1";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.frontBuffer)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox frontBuffer;
        private System.Windows.Forms.Timer timer1;
    }
}

