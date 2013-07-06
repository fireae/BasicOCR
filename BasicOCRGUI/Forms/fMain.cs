using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Drawing.Imaging;

using BasicOCR;
using BasicOCRControls;

namespace BasicOCRGUI
{
    public partial class fMain : Form
    {
        public ImageEdit ieMain;
        public DocDisplay ddMain;
        Bitmap mImage;
        List<CWord> mWords;
        CDocument ocr;
        BasicOCRGUI.Forms.fOCRText fwl;

        public fMain()
        {
            InitializeComponent();

            //this.tabControlMain.TabPages[0].Controls.Add(NewIeMain());

            /*
            //Added to get around Visual Studio bug when adding User Control
            this.panelMain.Controls.Add(ieMain);
            this.ieMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ieMain.Image = null;
            this.ieMain.Location = new System.Drawing.Point(0, 0);
            this.ieMain.Name = "ieMain";
            this.ieMain.Size = new System.Drawing.Size(457, 492);
            this.ieMain.TabIndex = 0;
            this.ieMain.Click += new System.EventHandler(this.ieMain_Click);
            this.ieMain.MouseUp += new System.Windows.Forms.MouseEventHandler(this.ieMain_MouseUp);
            */

            //this.tabControlMain.TabPages[0].Controls.Add(NewDocDisplay());

            /*
            this.panelMain.Controls.Add(ddMain);
            this.ddMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ddMain.Location = new System.Drawing.Point(0, 0);
            this.ddMain.Name = "ddMain";
            this.ddMain.Visible = false;
            */

            fwl = new BasicOCRGUI.Forms.fOCRText();
            fwl.Visible = false;
        }
        private void SetFormCaption(String fileName)
        {
            if (fileName.Length == 0)
                this.Text = "BasicOCR";
            else
                this.Text = "BasicOCR - " + Path.GetFileNameWithoutExtension(fileName);
        }
        private void ieMain_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e) {
            ImageEdit currentIe = (ImageEdit)(new Utils().FindControlRecursive(this.tabControlMain.SelectedTab, "ieMain"));

            if (currentIe == null) {
                return;                     // TODO: error handling
            }
            currentIe.SelectBox(e.X, e.Y);
            if (currentIe.SelectedObject != null)
                pgProperties.SelectedObject = currentIe.SelectedObject;
        }
        private void oCRTextToolStripMenuItem_Click_1(object sender, EventArgs e) {
        }
        private void imageToolStripMenuItem_Click(object sender, EventArgs e) {
            ImageEdit currentIe = (ImageEdit)(new Utils().FindControlRecursive(this.tabControlMain.SelectedTab, "ieMain"));

            if (currentIe == null) {
                return;                     // TODO: error handling
            }
            currentIe.Visible = true;
        }
        private void openToolStripMenuItem_Click(object sender, EventArgs e) {
            ImageEdit currentIe = (ImageEdit)(new Utils().FindControlRecursive(this.tabControlMain.SelectedTab, "ieMain"));

            if (currentIe == null) {
                return;                     // TODO: error handling
            }
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = @"OCR Files (*.ocr)|*.ocr|All Files (*.*)|*.*";
            dlg.RestoreDirectory = true;

            String FileName = (dlg.ShowDialog() == DialogResult.OK) ? dlg.FileName : string.Empty;

            if (FileName == string.Empty) return;

            ocr = currentIe.Load(FileName);
            currentIe.Refresh();
        }

        private ImageEdit NewIeMain() {
            ieMain = new ImageEdit();

            this.ieMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ieMain.Image = null;
            this.ieMain.Location = new System.Drawing.Point(0, 0);
            this.ieMain.Name = "ieMain";
            this.ieMain.Size = new System.Drawing.Size(457, 492);
            this.ieMain.TabIndex = 0;
            this.ieMain.Click += new System.EventHandler(this.ieMain_Click);
            this.ieMain.MouseUp += new System.Windows.Forms.MouseEventHandler(this.ieMain_MouseUp);

            return ieMain;
        }

        private DocDisplay NewDocDisplay() {
            ddMain = new DocDisplay();

            this.ddMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ddMain.Location = new System.Drawing.Point(0, 0);
            this.ddMain.Name = "ddMain";
            this.ddMain.Visible = false;

            return ddMain;
        }

        private void segmenterToolStripMenuItem_Click(object sender, EventArgs e) {
            fSegmenter frmSegmenter = new fSegmenter();
            frmSegmenter.ShowDialog();
        }

        private void tscbZoom_SelectIndexChanged(object sender, EventArgs e) {
            ImageEdit currentIe = (ImageEdit)(new Utils().FindControlRecursive(this.tabControlMain.SelectedTab, "ieMain"));

            if (currentIe == null) {
                return;                     // TODO: error handling
            }
            currentIe.ResizeImage(tscbZoom.Text);
        }

        private void tsbRecognize_Click(object sender, EventArgs e) {
            /*if (mImage != null && !string.IsNullOrEmpty(ieMain.ImagePath)) {
                ieMain.SetOCR(ocr = new CDocument());

                progressBar1.Value = 0;
                //listBox1.Items.Clear();

                ocr.TesseractInit("E:\\dev\\BasicOCR\\builds\\Debug", "eng", false);
                ocr.ProgressEvent += new CDocument.ProgressHandler(ocrProgressEvent);
                ocr.OcrDone = new CDocument.OcrDoneHandler(Done);
                ocr.TesseractRecognize(mImage, Rectangle.Empty);
            }*/
        }

        private void tsbShowParagraphs_Click(object sender, EventArgs e) {
            ImageEdit currentIe = (ImageEdit)(new Utils().FindControlRecursive(this.tabControlMain.SelectedTab, "ieMain"));

            if (currentIe == null) {
                return;                     // TODO: error handling
            }

            if (currentIe.ShowParagraphs)
                currentIe.ShowParagraphs = false;
            else
                currentIe.ShowParagraphs = true;
        }

        private void tsbShowLines_Click(object sender, EventArgs e) {
            ImageEdit currentIe = (ImageEdit)(new Utils().FindControlRecursive(this.tabControlMain.SelectedTab, "ieMain"));

            if (currentIe == null) {
                return;                     // TODO: error handling
            }

            if (currentIe.ShowLines)
                currentIe.ShowLines = false;
            else
                currentIe.ShowLines = true;
        }

        private void tsbShowColumns_Click(object sender, EventArgs e) {
            ImageEdit currentIe = (ImageEdit)(new Utils().FindControlRecursive(this.tabControlMain.SelectedTab, "ieMain"));

            if (currentIe == null) {
                return;                     // TODO: error handling
            }

            if (currentIe.ShowColumns)
                currentIe.ShowColumns = false;
            else
                currentIe.ShowColumns = true;
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e) {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = @"TIFF Files (*.tif)|*.tif|PNG Files (*.png)|*.png|JPEG Files (*.jpg)|*.jpg|All Files (*.*)|*.*";
            dlg.RestoreDirectory = true;

            String FileName = (dlg.ShowDialog() == DialogResult.OK) ? dlg.FileName : string.Empty;

            if (FileName == string.Empty) return;
            
            
            string title = "New Job " + (tabControlMain.TabCount + 1).ToString();

            TabPage newTabPage = new TabPage(title);

            newTabPage.Name = "ocrTab";

            ImageEdit ie = NewIeMain();

            tabControlMain.TabPages.Add(newTabPage);
            newTabPage.Controls.Add(ie);

            ie.ImagePath = dlg.FileName;
            ie.ResizeImage( tscbZoom.Text = ( ( (int)( ( (float)ieMain.Width / 
                (float)ie.Image.Size.Width ) * 100f ) + "%").ToString() ) );
            mImage = new Bitmap(dlg.FileName);
            mImage.SetResolution(96, 96);

            ie.Refresh();
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e) {
            SaveFileDialog dlg = new SaveFileDialog();
            dlg.Filter = @"OCR Files (*.ocr)|*.ocr|All Files (*.*)|*.*";
            dlg.RestoreDirectory = true;

            String FileName = (dlg.ShowDialog() == DialogResult.OK) ? dlg.FileName : string.Empty;

            if (FileName == string.Empty) return;

            ocr.Save(FileName);
        }

        private void closeToolStripMenuItem_Click(object sender, EventArgs e) {
            ImageEdit currentIe = (ImageEdit)(new Utils().FindControlRecursive(this.tabControlMain.SelectedTab, "ieMain"));

            if (currentIe == null)
            {
                return;                     // TODO: error handling
            }

            currentIe.Close();
            tsbShowColumns.Checked = true;
            tsbShowLines.Checked = true;
            tsbShowParagraphs.Checked = true;
            tsbShowCharacters.Checked = true;
            tabControlMain.SelectedTab.Dispose();
        }

        private void toolStripMenuItem1_Click(object sender, EventArgs e) {
            SaveFileDialog dlg = new SaveFileDialog();
            dlg.Filter = @"PDF Files (*.pdf)|*.pdf|All Files (*.*)|*.*";
            dlg.RestoreDirectory = true;

            String FileName = (dlg.ShowDialog() == DialogResult.OK) ? dlg.FileName : string.Empty;

            if (FileName == string.Empty) return;
            
            ImageEdit currentIe = (ImageEdit)(new Utils().FindControlRecursive(this.tabControlMain.SelectedTab, "ieMain"));

            if (currentIe == null)
            {
                return;                     // TODO: error handling
            }

            ocr = currentIe.GetOCR();

            if (ocr == null) {
                return;
            }

            ocr.Export(FileName);
        }

        private void fileToolStripMenuItem_Click(object sender, EventArgs e) {

        }
        private void ieMain_Click(object sender, EventArgs e) {

        }

        void Done(List<CWord> words) {
            mWords = words;
            this.Invoke(new FillResult(FillResultMethod));
        }

        delegate void SetPercent(int percent);

        void ocrProgressEvent(int percent) {
            progressBar1.Invoke(new SetPercent(SetPercentMethod), new object[] { percent });
        }
        void SetPercentMethod(int percent) {
            progressBar1.Value = percent;
        }

        delegate void FillResult();

        private void FillResultMethod() {
            ImageEdit currentIe = (ImageEdit)(new Utils().FindControlRecursive(this.tabControlMain.SelectedTab, "ieMain"));

            if (currentIe == null) {
                return;                     // TODO: error handling
            }

            String text = currentIe.GetOCRText();
            fwl.SetText(text);

            progressBar1.Value = 0;
            currentIe.Invalidate();
            currentIe.Refresh();
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e) {
            //TODO: check change history
            if (DialogResult.Yes == MessageBox.Show(
                "Recent changes have not been saved. Close the application anyway?",
                "Close Application?",
                 MessageBoxButtons.YesNo,
                 MessageBoxIcon.Question,
                 MessageBoxDefaultButton.Button2)) {
                Application.Exit();
            }
        }

        private void toolStripButton1_Click(object sender, EventArgs e) {
            ImageEdit currentIe = (ImageEdit)(new Utils().FindControlRecursive(this.tabControlMain.SelectedTab, "ieMain"));

            if (currentIe == null) {
                return;                     // TODO: error handling
            }
             
            if (currentIe.ShowCharacters)
                currentIe.ShowCharacters = false;
            else
                currentIe.ShowCharacters = true;
        }

        private void exportToolStripMenuItem_Click(object sender, EventArgs e) {

        }
        private void aboutToolStripMenuItem_Click(object sender, EventArgs e) {

        }

        private void pgProperties_Click(object sender, EventArgs e) {

        }

        private void richTextBox1_TextChanged(object sender, EventArgs e) {

        }

        private void viewToolStripMenuItem_Click(object sender, EventArgs e) {

        }

        private void toolStripButton3_Click(object sender, EventArgs e) {
            ImageEdit currentIe = (ImageEdit)(new Utils().FindControlRecursive(this.tabControlMain.SelectedTab, "ieMain"));

            if (currentIe == null) {
                return;                     // TODO: error handling
            }

            if (mImage != null && !string.IsNullOrEmpty(currentIe.ImagePath)) {
                ocr = null;
                ocr = currentIe.GetOCR();// new CDocument();

                if (ocr == null) {
                    return;             // TODO: error handling
                }

                progressBar1.Value = 0;
                //listBox1.Items.Clear();

                ocr.OcrDone = new CDocument.OcrDoneHandler(Done);
                ocr.ProgressEvent += new CDocument.ProgressHandler(ocrProgressEvent);
                currentIe.GetOCR().OCRopusLayoutAnalysis();
            }
        }

        private void toolStrip1_ItemClicked(object sender, ToolStripItemClickedEventArgs e) {

        }

        private void tscbZoom_Click(object sender, EventArgs e) {

        }

        private void toolStripContainer2_TopToolStripPanel_Click(object sender, EventArgs e) {

        }

        private void toolsToolStripMenuItem_Click(object sender, EventArgs e) {

        }


        private void wordListToolStripMenuItem_Click(object sender, EventArgs e) {
            ImageEdit currentIe = (ImageEdit)(new Utils().FindControlRecursive(this.tabControlMain.SelectedTab, "ieMain"));

            if (currentIe == null) {
                return;                     // TODO: error handling
            }
            fwl.SetText(currentIe.GetOCRText());

            this.wordListToolStripMenuItem.Checked = !this.wordListToolStripMenuItem.Checked;

            fwl.Visible = this.wordListToolStripMenuItem.Checked;
        }

        private void panelMain_Paint(object sender, PaintEventArgs e) {

        }

        private void toolStripContainer2_ContentPanel_Load(object sender, EventArgs e) {

        }

        private void tabPage1_Click(object sender, EventArgs e) {

        }

        private void tabControlMain_SelectedIndexChanged(object sender, EventArgs e)
        {
            ImageEdit currentIe = (ImageEdit)(new Utils().FindControlRecursive(this.tabControlMain.SelectedTab, "ieMain"));

            if (currentIe == null) {
                return;                     // TODO: error handling
            }
            fwl.SetText(currentIe.GetOCRText());

            tsbShowColumns.Checked = currentIe.ShowColumns;
            tsbShowParagraphs.Checked = currentIe.ShowParagraphs;
            tsbShowLines.Checked = currentIe.ShowLines;
            tsbShowCharacters.Checked = currentIe.ShowCharacters;

            tscbZoom.Text = ((int)(currentIe.Zoom * 100.0)).ToString() + "%";
        }

    }
}
