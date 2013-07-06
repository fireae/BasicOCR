using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using BasicOCR;

namespace BasicOCRGUI
{
    public partial class fSegmenter : Form
    {
        public fSegmenter()
        {
            InitializeComponent();
        }

        private void btnSegment_Click(object sender, EventArgs e)
        {
            CDocument OCRDoc = new CDocument(tbInput.Text);

            OCRDoc.Segment();
            OCRDoc.Save(tbOutput.Text);
        }

        private void btnBrowseInput_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = @"TIFF Files (*.tif)|*.tif|PNG Files (*.png)|*.png|JPEG Files (*.jpg)|*.jpg|All Files (*.*)|*.*";
            dlg.RestoreDirectory = true;

            tbInput.Text = (dlg.ShowDialog() == DialogResult.OK) ? dlg.FileName : string.Empty;
        }

        private void btnBrowseOutput_Click(object sender, EventArgs e)
        {
            SaveFileDialog dlg = new SaveFileDialog();
            dlg.Filter = @"XML Files (*.xml)|*.xml|All Files (*.*)|*.*";
            dlg.RestoreDirectory = true;

            tbOutput.Text = (dlg.ShowDialog() == DialogResult.OK) ? dlg.FileName : string.Empty;
        }
    }
}
