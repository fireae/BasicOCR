using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace BasicOCRGUI.Forms
{
    public partial class fOCRText : Form
    {
        public fOCRText()
        {
            InitializeComponent();
        }

        private void fWordList_Load(object sender, EventArgs e)
        {

        }

        private void richTextBox1_TextChanged(object sender, EventArgs e)
        {
        }

        public void SetText(String text)
        {
            this.rtbOCRText.Clear();
            this.rtbOCRText.Text = text;
        }
    }
}
