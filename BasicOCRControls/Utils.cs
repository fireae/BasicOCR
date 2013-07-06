using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace BasicOCRControls
{
    static class Utils
    {
        public static Rectangle FixRect(int x, int y, int width, int height, int imageHeight)
        {
            Rectangle rect = new Rectangle();
            rect.X = x;
            rect.Y = imageHeight - y;
            rect.Width = width;
            rect.Height = height;
            rect.Y = rect.Y - rect.Height;

            return rect;
        }
    }
}
