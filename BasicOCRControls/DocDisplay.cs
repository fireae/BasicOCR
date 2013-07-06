using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Drawing.Imaging;
using BasicOCR;

namespace BasicOCRControls
{
    public partial class DocDisplay : ScrollableControl
    {
        private float _zoom;
        private CDocument _ocr;
        private List<RichTextBox> rtbColumns = new List<RichTextBox>();
        private int _docheight;
        private int _docwidth;

        #region Constructor

        public DocDisplay()
        {
            InitializeComponent();
            SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            SetStyle(ControlStyles.UserPaint, true);
            SetStyle(ControlStyles.DoubleBuffer, true);
            SetStyle(ControlStyles.ResizeRedraw, true);
            _zoom = 1;
        }

        #endregion

        #region Private

        private void UpdateScaleFactor()
        {
            AutoScrollMinSize = new Size((int)(_docwidth * _zoom + 0.5F), (int)(_docheight * _zoom + 0.5F));
            //AutoScrollMinSize = Size;
        }

        #endregion

        #region Public

        public void ResizeImage(string percentage)
        {
            percentage = percentage.Replace("%", "");
            ResizeImage(Convert.ToSingle(percentage));
        }

        public void ResizeImage(float percentage)
        {
            _zoom = percentage / 100F;
            UpdateScaleFactor();
            Invalidate();
        }

        public void SetOCRDoc(CDocument OCRObj)
        {
            _ocr = OCRObj;

            foreach (CColumn curColumn in _ocr.Columns)
            {
                RichTextBox rtbToAdd = new RichTextBox();
                this.Controls.Add(rtbToAdd);
                Rectangle rect = Utils.FixRect(curColumn.X, curColumn.Y, curColumn.Width, curColumn.Height, _docheight);
                rtbToAdd.Left = rect.X;
                rtbToAdd.Top = rect.Y;
                rtbToAdd.Width = rect.Width;
                rtbToAdd.Height = rect.Height;
                rtbToAdd.BorderStyle = BorderStyle.None;
                foreach (CParagraph curParagraph in curColumn.Paragraphs)
                {
                    rtbToAdd.Text += (curParagraph.Text + "\r\n");
                }
            }

            UpdateScaleFactor();

        }


        #endregion

        #region Properties

        public int DocHeight
        {
            get { return _docheight; }
            set { _docheight = value; }
        }

        public int DocWidth
        {
            get { return _docwidth; }
            set { _docwidth = value; }
        }

        #endregion

        #region Private/Protected

        protected override void OnPaintBackground(PaintEventArgs e)
        {

        }

        protected override void OnPaint(PaintEventArgs e)
        {

            /*Matrix mx = new Matrix(_zoom, 0, 0, _zoom, 0, 0);
            mx.Translate(AutoScrollPosition.X / _zoom, AutoScrollPosition.Y / _zoom);
            e.Graphics.Transform = mx;
            e.Graphics.InterpolationMode = _interpolationMode;
            e.Graphics.DrawImage(_image, new Rectangle(0, 0, _image.Width, _image.Height), 0, 0, _image.Width, _image.Height, GraphicsUnit.Pixel);*/

            base.OnPaint(e);

        }

        #endregion

    }
}

