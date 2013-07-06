namespace BasicOCRGUI.Forms
{
    partial class fOCRText
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
            this.rtbOCRText = new System.Windows.Forms.RichTextBox();
            this.SuspendLayout();
            // 
            // rtbOCRText
            // 
            this.rtbOCRText.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.rtbOCRText.Location = new System.Drawing.Point(1, 3);
            this.rtbOCRText.Name = "rtbOCRText";
            this.rtbOCRText.Size = new System.Drawing.Size(1014, 563);
            this.rtbOCRText.TabIndex = 0;
            this.rtbOCRText.Text = "";
            this.rtbOCRText.TextChanged += new System.EventHandler(this.richTextBox1_TextChanged);
            // 
            // fOCRText
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1017, 567);
            this.ControlBox = false;
            this.Controls.Add(this.rtbOCRText);
            this.Name = "fOCRText";
            this.Text = "OCR Text";
            this.Load += new System.EventHandler(this.fWordList_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.RichTextBox rtbOCRText;

    }
}