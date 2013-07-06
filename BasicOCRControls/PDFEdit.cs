/*using it.stefanochizzolini.clown.documents;
using it.stefanochizzolini.clown.documents.contents.composition;
using it.stefanochizzolini.clown.documents.contents.fonts;
using it.stefanochizzolini.clown.files;

using BasicOCR;
using BasicOCRControls;

using System;
using System.Drawing;

namespace BasicOCR
{
    
    public class PDFManipulator
      : ISample
    {
        #region dynamic
        #region interface
        #region public
        #region ISample
        public void Export( PDFClownSampleLoader loader, String Filename, CDocument ocrDocument )//Gary - for pdf export; for window stream change filename to stream
        {
            // Instantiate a new PDF file!
            File file = new File(Filename);
            
            // Get its corresponding document!
            Document document = file.Document;

            // Insert the contents into the document!
            Populate(document, ocrDocument);

            // (boilerplate metadata insertion -- ignore it)
            loader.BuildAccessories(document, this.GetType(), "NewPage", "Page");

            // Serialize the PDF file (again, boilerplate code -- see the PDFClownSampleLoader class source code)!
            loader.Serialize(file, this.GetType().Name, false);
        }
        #endregion
        #endregion

        #region private
      
        private void Populate(Document document, CDocument ocrDocument)
        {
            // 1. Add the page to the document!
            Page page = new Page(document); // Instantiates the page inside the document context.
            document.Pages.Add(page); // Puts the page in the pages collection.

            // 2. Create a content builder for the page!
            PrimitiveFilter builder = new PrimitiveFilter(page);

            // 3. Inserting contents...
            // Set the font to use!
            builder.SetFont(new StandardType1Font(document, 
                                StandardType1Font.FamilyNameEnum.Courier,
                                true,
                                false ), 32);

            // Show the text onto the page!
            foreach (CLine Line in ocrDocument.Lines)
            {
                builder.ShowText(Line.ToString() , new PointF(Line.x, Line.y));
            }

            // 4. Flush the contents into the page!
            builder.Flush();
        }
        #endregion
        #endregion
        #endregion
    }
}*/