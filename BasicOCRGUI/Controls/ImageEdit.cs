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
using System.Reflection;

namespace OcropusGUI
{
    public enum BBox { Paragraph, Column, Line, Character, Word, Margin, Header, Footer };

    public partial class ImageEdit : ScrollableControl
    {

        private Image _image = null;
        private float _zoom;
        private InterpolationMode _interpolationMode = InterpolationMode.High;
        private string _imagepath;
        private bool _showcolumns = true;
        private bool _showparagraphs = true;
        private bool _showlines = true;
        private bool _showchars = true;
        private List<Rectangle> _columns;
        private List<Rectangle> _paragraphs;
        private List<Rectangle> _lines;
        private List<Rectangle> _characters;

        public ImageEdit()
        {
            InitializeComponent();
            SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            SetStyle(ControlStyles.UserPaint, true);
            SetStyle(ControlStyles.DoubleBuffer, true);
            SetStyle(ControlStyles.ResizeRedraw, true);
            _zoom = 1;
            ResetBoxes();
        }

        #region Properties

        public Image Image {
            get { return _image; }
            set {
                _image = value;
                UpdateScaleFactor();
                Invalidate();
            }
        }

        public string ImagePath
        {
            set
            {
                _image = Image.FromFile(value);
                string tempFileName = Path.GetTempFileName();
                _image.Save(tempFileName, ImageFormat.Jpeg);
                _image = Image.FromFile(tempFileName);
                //_ocr = new CDocument(value);
                _imagepath = tempFileName;
                UpdateScaleFactor();
                Invalidate();
            }

        }

        public float Zoom
        {
            get { return _zoom; }
            set
            {
                _zoom = value;
                UpdateScaleFactor();
                Invalidate();
            }
        }

        public bool ShowParagraphs
        {
            get { return _showparagraphs; }
            set {
                _showparagraphs = value;
                UpdateScaleFactor();
                Invalidate();
            }
        }

        public bool ShowLines
        {
            get { return _showlines; }
            set {
                _showlines = value;
                UpdateScaleFactor();
                Invalidate();
            }
        }

        public bool ShowCharacters
        {
            get { return _showchars; }
            set
            {
                _showchars = value;
                UpdateScaleFactor();
                Invalidate();
            }
        }

        public bool ShowColumns
        {
            get { return _showcolumns; }
            set {
                _showcolumns = value;
                UpdateScaleFactor();
                Invalidate();
            }
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

        /*public void Recognize()
        {
            //_ocr.Segment();
            _ocr.RecognizeTesseract();
            UpdateScaleFactor();
            Invalidate();
        }*/

        /*public void Load(string filePath)
        {
            _ocr = CDocument.Load(filePath);
            _imagepath = _ocr.ImagePath;
            _image = Image.FromFile(_ocr.ImagePath);
            UpdateScaleFactor();
            Invalidate();
        }*/

        /*public void Save(string filePath)
        {
            _ocr.Save(filePath);
        }*/

        public void Close()
        {
            //_ocr = null;
            _imagepath = string.Empty;
            _showcolumns = true;
            _showparagraphs = true;
            _showlines = true;
            _image = null;
            UpdateScaleFactor();
            Invalidate();
        }

        public void ResetBoxes()
        {
            _columns = new List<Rectangle>();
            _paragraphs = new List<Rectangle>();
            _lines = new List<Rectangle>();
            _characters = new List<Rectangle>();
        }

        public void AddBox(int x, int y, int width, int height, BBox type)
        {
            switch (type)
            {
                case BBox.Paragraph:
                    _paragraphs.Add(FixRect(x, y, width, height));
                    break;
                case BBox.Column:
                    _columns.Add(FixRect(x, y, width, height));
                    break;
                case BBox.Line:
                    _lines.Add(FixRect(x, y, width, height));
                    break;
                case BBox.Character:
                    _characters.Add(FixRect(x, y, width, height));
                    break;
            }
        }

        #endregion

        #region Private

        private void UpdateScaleFactor()
        {
            if (_image != null)
            {
                AutoScrollMinSize = new Size((int)(_image.Width * _zoom + 0.5F), (int)(_image.Height * _zoom + 0.5F));
            }
            else
            {
                AutoScrollMinSize = Size;
            }

        }

        private Rectangle FixRect(int x, int y, int width, int height)
        {
            Rectangle rect = new Rectangle();
            rect.X = x;
            rect.Y = Image.Height - y;
            rect.Width = width;
            rect.Height = height;
            rect.Y = rect.Y - rect.Height;

            return rect;
        }

        #endregion

        #region Private/Protected

        protected override void OnPaintBackground(PaintEventArgs e)
        {
            
        }

        protected override void OnPaint(PaintEventArgs e) {
            if (Image != null)
            {
                Matrix mx = new Matrix(_zoom, 0, 0, _zoom, 0, 0);
                mx.Translate(AutoScrollPosition.X / _zoom, AutoScrollPosition.Y / _zoom);
                e.Graphics.Transform = mx;
                e.Graphics.InterpolationMode = _interpolationMode;
                e.Graphics.DrawImage(_image, new Rectangle(0, 0, _image.Width, _image.Height), 0, 0, _image.Width, _image.Height, GraphicsUnit.Pixel);

                /*if (_ocr != null)
                {
                    if (_showparagraphs)
                    {
                        foreach (CParagraph curParagraph in _ocr.Paragraphs)
                        {
                            Rectangle rect = FixRect(curParagraph.x, curParagraph.y, curParagraph.width, curParagraph.height);
                            e.Graphics.FillRectangle(new SolidBrush(Color.FromArgb(40, 0, 0, 255)), rect);
                            e.Graphics.DrawRectangle(new Pen(Color.FromArgb(255, 0, 0, 255)), rect);
                        }
                    }

                    if (_showlines)
                    {
                        foreach (CLine curLine in _ocr.Lines)
                        {
                            Rectangle rect = FixRect(curLine.x, curLine.y, curLine.width, curLine.height);
                            e.Graphics.FillRectangle(new SolidBrush(Color.FromArgb(40, 0, 255, 0)), rect);
                            e.Graphics.DrawRectangle(new Pen(Color.FromArgb(255, 0, 255, 0)), rect);
                        }
                    }

                    if (_showcolumns)
                    {
                        foreach (CColumn curColumn in _ocr.Columns)
                        {
                            Rectangle rect = FixRect(curColumn.x, curColumn.y, curColumn.width, curColumn.height);
                            e.Graphics.FillRectangle(new SolidBrush(Color.FromArgb(40, 255, 0, 0)), rect);
                            e.Graphics.DrawRectangle(new Pen(Color.FromArgb(255, 255, 0, 0)), rect);
                        }
                    }
                }*/
                base.OnPaint(e);
            }
            else
            {
                base.OnPaintBackground(e);
            }
        }

        #endregion


    }
}
