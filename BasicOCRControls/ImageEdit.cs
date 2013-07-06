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
    public partial class ImageEdit : ScrollableControl
    {

        private Image _image = null;
        private float _zoom;
        private InterpolationMode _interpolationMode = InterpolationMode.High;
        private CDocument _ocr = null;
        private string _imagepath;
        private bool _showcolumns = true;
        private bool _showparagraphs = true;
        private bool _showlines = true;
        private bool _showcharacters = true;
        private int _xSelection = -1;
        private int _ySelection = -1;
        private CDocObject _selectionObject = null;
        private bool _wordselect = false;//notice CDocObject
        private CWord _SelectedWord;
        //private PDFManipualter _PDFEdit;

        public ImageEdit()
        {
            InitializeComponent();
            SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            SetStyle(ControlStyles.UserPaint, true);
            SetStyle(ControlStyles.DoubleBuffer, true);
            SetStyle(ControlStyles.ResizeRedraw, true);
            _zoom = 1;
        }

        public void SetOCR( CDocument ocr )
        {
            _ocr = ocr;
        }

        public CDocument GetOCR()
        {
            return _ocr;
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
                _ocr = new CDocument(value);
                _imagepath = tempFileName;
                UpdateScaleFactor();
                Invalidate();
            }
            get
            {
                return _imagepath;
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

        public bool ShowColumns
        {
            get { return _showcolumns; }
            set {
                _showcolumns = value;
                UpdateScaleFactor();
                Invalidate();
            }
        }

        public bool ShowCharacters
        {
            get { return _showcharacters; }
            set
            {
                _showcharacters = value;
                UpdateScaleFactor();
                Invalidate();
            }
        }
        public object SelectedObject
        {
            get { return _selectionObject; }
        }
        public CDocument OCRObject
        {
            get { return _ocr; }
        }

        #endregion

        #region Public

        public string GetOCRText()
        {

            if (_ocr == null)
                return "empty";

            String sOCRText = "";

            //foreach (CColumn curColumn in _ocr.Columns)
            //    foreach (CParagraph curParagraph in curColumn.Paragraphs)
                    foreach (CLine curLine in _ocr.Lines)
                        sOCRText += (curLine.LineText + "\r\n");

            return sOCRText;

        }
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

        public void Close()
        {
            _ocr = null;
            _imagepath = string.Empty;
            _showcolumns = true;
            _showparagraphs = true;
            _showlines = true;
            _showcharacters = true;
            _image = null;
            UpdateScaleFactor();
            Invalidate();
        }

        public void SelectBox(int x, int y)
        {
            _xSelection = (int)((x - AutoScrollPosition.X) * (1F / _zoom) + 0.5F);
            _ySelection = (int)((y - AutoScrollPosition.Y) * (1F / _zoom) + 0.5F);
            SetSelectionObject();
            UpdateScaleFactor();
            Invalidate();
        }

        public CDocument Load(string filePath)
        {
            _ocr = CDocument.Load(filePath);
            _imagepath = _ocr.ImagePath;
            _image = Image.FromFile(_ocr.ImagePath);
            UpdateScaleFactor();
            Invalidate();
            return _ocr;
        }

        public void SetSelectedWord(CWord tWord, bool tshow)
        {
            _SelectedWord = tWord;
            _wordselect = tshow;
            UpdateScaleFactor();
            Invalidate();
        }
        #endregion
        private void SetSelectionObject()
        {

            if (_ocr != null)
            {
                if (_showparagraphs)
                {
                    foreach (CParagraph curParagraph in _ocr.Paragraphs)
                    {
                        //Rectangle rect = Utils.FixRect(curParagraph.X, curParagraph.Y, curParagraph.Width, curParagraph.Height, _image.Height);
                        Rectangle rect = new Rectangle(curParagraph.X, curParagraph.Y, curParagraph.Width, curParagraph.Height);
                        if (IsSelected(rect))
                            _selectionObject = curParagraph;
                    }
                }

                if (_showlines)
                {
                    foreach (CLine curLine in _ocr.Lines)
                    {
                        //Rectangle rect = Utils.FixRect(curLine.X, curLine.Y, curLine.Width, curLine.Height, _image.Height);
                        Rectangle rect = new Rectangle(curLine.X, curLine.Y, curLine.Width, curLine.Height);
                        if (IsSelected(rect))
                            _selectionObject = curLine;
                    }
                }

                if (_showcolumns)
                {
                    foreach (CColumn curColumn in _ocr.Columns)
                    {
                        //Rectangle rect = Utils.FixRect(curColumn.X, curColumn.Y, curColumn.Width, curColumn.Height, _image.Height);
                        Rectangle rect = new Rectangle(curColumn.X, curColumn.Y, curColumn.Width, curColumn.Height);
                        if (IsSelected(rect))
                            _selectionObject = curColumn;

                    }
                }

                if (_showcharacters)
                {
                    foreach (CCharacter curChar in _ocr.Characters)
                    {
                        //Rectangle rect = Utils.FixRect(curChar.X, curChar.Y, curChar.Width, curChar.Height, _image.Height);
                        Rectangle rect = new Rectangle(curChar.X, curChar.Y, curChar.Width, curChar.Height);
                        if (IsSelected(rect))
                            _selectionObject = curChar;
                    }
                }
            }
        }

        private bool IsSelected(Rectangle rect)
        {
            if ((_xSelection > rect.X) && (_xSelection < rect.X + rect.Width) &&
                (_ySelection > rect.Y) && (_ySelection < rect.Y + rect.Height))
                return true;
            return false;
        }

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

                if (_ocr != null)
                {
                    if (_showparagraphs)
                    {
                        foreach (CParagraph curParagraph in _ocr.Paragraphs)
                        {
                            Rectangle rect = new Rectangle(curParagraph.X, curParagraph.Y, curParagraph.Width, curParagraph.Height);
                            e.Graphics.FillRectangle(new SolidBrush(Color.FromArgb(40, 0, 0, 255)), rect);
                            e.Graphics.DrawRectangle(new Pen(Color.FromArgb(255, 0, 0, 255)), rect);
                        }
                    }

                    if (_showlines)
                    {
                        foreach (CLine curLine in _ocr.Lines)
                        {
                            Rectangle rect = new Rectangle(curLine.X, curLine.Y, curLine.Width, curLine.Height);
                            e.Graphics.FillRectangle(new SolidBrush(Color.FromArgb(40, 0, 255, 0)), rect);
                            e.Graphics.DrawRectangle(new Pen(Color.FromArgb(255, 0, 255, 0)), rect);
                        }
                    }

                    if (_showcolumns)
                    {
                        foreach (CColumn curColumn in _ocr.Columns)
                        {
                            Rectangle rect = new Rectangle(curColumn.X, curColumn.Y, curColumn.Width, curColumn.Height);
                            e.Graphics.FillRectangle(new SolidBrush(Color.FromArgb(40, 255, 0, 0)), rect);
                            e.Graphics.DrawRectangle(new Pen(Color.FromArgb(255, 255, 0, 0)), rect);
                        }
                    }

                    if (_showcharacters)
                    {
                        foreach (CCharacter curChar in _ocr.Characters)
                        {
                            Rectangle rect = new Rectangle(curChar.X, curChar.Y, curChar.Width, curChar.Height);
                            e.Graphics.FillRectangle(new SolidBrush(Color.FromArgb(40, 255, 128, 0)), rect);
                            e.Graphics.DrawRectangle(new Pen(Color.FromArgb(255, 255, 128, 0)), rect);
                        }

                    }
                    if (_selectionObject != null)
                    {
                        //Rectangle rect = Utils.FixRect(_selectionObject.X, _selectionObject.Y, _selectionObject.Width, _selectionObject.Height, _image.Height);
                        Rectangle rect = new Rectangle(_selectionObject.X, _selectionObject.Y, _selectionObject.Width, _selectionObject.Height);
                        e.Graphics.FillRectangle(new SolidBrush(Color.FromArgb(40, 0, 0, 0)), rect);
                        e.Graphics.DrawRectangle(new Pen(Color.FromArgb(255, 0, 0, 0), 5), rect);
                    }
                    if (_wordselect)
                    {
                        Rectangle rect = new Rectangle(_SelectedWord.Left, _SelectedWord.Top, _SelectedWord.Width -  _SelectedWord.Left, _SelectedWord.Height - _SelectedWord.Top);
                        e.Graphics.FillRectangle(new SolidBrush(Color.FromArgb(40, 60, 0, 255)), rect);
                        e.Graphics.DrawRectangle(new Pen(Color.FromArgb(255, 60, 0, 255)), rect);
                    }

                }
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
