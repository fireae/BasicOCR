using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

//using OcropusWrapper;

namespace OcropusGUI
{
    public partial class ImageEdit : UserControl
    {

        private Image _image = null;
        private Matrix _transform;
        private float _scale;

        public ImageEdit()
        {
            InitializeComponent();
            SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            SetStyle(ControlStyles.UserPaint, true);
            SetStyle(ControlStyles.DoubleBuffer, true);
            _transform = new Matrix();
            _transform.Scale(1, 1);
            _scale = 1;
        }

        #region Properties

        public Image Image {
            get { return _image; }
            set {
                _image = value;
            }
        }

        public Matrix Transform
        {
            get { return _transform; }
        }

        #endregion

        #region Public

        public void SetupAutoScroll() {
            if(_image != null) {
                // Get the image size
                SizeF s = SizeF.Empty;
                s.Width = (float)96.0 * Image.Width / Image.HorizontalResolution;
                s.Height =(float)96.0 * Image.Height / Image.VerticalResolution;
       
                // calculate the current scaled size of image
                PointF[] pts = new PointF[1];
                pts[0].X = s.Width;
                pts[0].Y = s.Height;
                _transform.TransformPoints(pts);

                // that's your autoscroll size
                AutoScrollMinSize = new Size((int)(pts[0].X + 0.5), (int)(pts[0].Y + 0.5));
            } else {
                // No image, no scrollbars
                AutoScrollMinSize = Size.Empty;
            }
        }

        public void ResizeImage(string percentage)
        {
            percentage = percentage.Replace("%", "");
            ResizeImage(Convert.ToSingle(percentage));
        }

        public void ResizeImage(float percentage)
        {
            float scale = (percentage / 100F) / _scale;
            _scale = percentage / 100F;
            Transform.Scale(scale, scale);
            Invalidate();
        }

        #endregion

        #region Private/Protected

        protected override void OnPaint(PaintEventArgs e) {
            if(Image != null) {
                // first, scroll then transform
                //e.Graphics.TranslateTransform(AutoScrollPosition.X, AutoScrollPosition.Y);
                e.Graphics.MultiplyTransform(Transform);
                e.Graphics.DrawImage(_image, 0, 0);

                /*foreach(CParagraph curParagraph in _ocr.Paragraphs)
                {
                    Rectangle rect = new Rectangle((int)(curParagraph.x/_image.HorizontalResolution), (int)(curParagraph.y/_image.VerticalResolution),
                                    (int)(curParagraph.width/_image.HorizontalResolution), (int)(curParagraph.height/_image.VerticalResolution));
                    e.Graphics.FillRectangle(new SolidBrush(Color.FromArgb(40, 0, 0, 255)), rect);
                    e.Graphics.DrawRectangle(new Pen(Color.FromArgb(255, 0, 0, 255)), rect);
                }

                foreach(CLine curLine in _ocr.Lines)
                {
                    Rectangle rect = new Rectangle((int)(curLine.x/_image.HorizontalResolution), (int)(curLine.y/_image.VerticalResolution),
                                    (int)(curLine.width/_image.HorizontalResolution), (int)(curLine.height/_image.VerticalResolution));
                    e.Graphics.FillRectangle(new SolidBrush(Color.FromArgb(40, 0, 255, 0)), rect);
                    e.Graphics.DrawRectangle(new Pen(Color.FromArgb(255, 0, 255, 0)), rect);
                }

                foreach(CColumn curColumn in _ocr.Columns)
                {
                    Rectangle rect = new Rectangle((int)(curColumn.x/_image.HorizontalResolution), (int)(curColumn.y/_image.VerticalResolution),
                                    (int)(curColumn.width/_image.HorizontalResolution), (int)(curColumn.height/_image.VerticalResolution));
                    e.Graphics.FillRectangle(new SolidBrush(Color.FromArgb(40, 255, 0, 0)), rect);
                    e.Graphics.DrawRectangle(new Pen(Color.FromArgb(255, 255, 0, 0)), rect);
                }*/
            }
        }

        #endregion

        private void ImageEdit_Load(object sender, EventArgs e)
        {

        }


    }
}
