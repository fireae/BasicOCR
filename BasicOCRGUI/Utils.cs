using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace BasicOCRGUI
{
    class Utils
    {
        public Control FindControlRecursive(Control container, string name)
        {
            if (container == null) return null;

            if (container.Name == name) return container;

            foreach (Control ctrl in container.Controls)
            {
                Control foundCtrl = FindControlRecursive(ctrl, name);

                if (foundCtrl != null) return foundCtrl;
            }

            return null;
        }
    }
}
