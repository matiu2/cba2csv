/*
 * =====================================================================================
 *
 *       Filename:  pdf2txt.hpp
 *
 *    Description:  Turns a pdf into a vector strings
 *
 *        Version:  1.0
 *        Created:  07/06/2011 06:56:57 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Matthew Sherborne (), msherborne@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef PDF2txt_HPP
#define PDF2txt_HPP

#include <podofo/base/PdfDefines.h>
#include <podofo/podofo.h>
#include <string>
#include <vector>

namespace {
    using std::string;
    using PoDoFo::PdfMemDocument;
    using PoDoFo::PdfContentsTokenizer;
    using PoDoFo::EPdfContentsType;
    using PoDoFo::PdfPage;
    using PoDoFo::PdfVariant;
    using PoDoFo::ePdfDataType_String;
}

namespace pdf2txt {

void pdf2txt(char* pdfFilename, std::vector<string>& result) {
    PdfMemDocument document( pdfFilename );

    int pageCount = document.GetPageCount();
    for( int i=0; i<pageCount; ++i ) 
    {
        PdfPage* page = document.GetPage( i );
        const char*      token = NULL;
        PdfVariant       var;
        EPdfContentsType tokenType;

        PdfContentsTokenizer tokenizer( page );
        while( tokenizer.ReadNext( tokenType, token, var ) ) {
            if ((var.GetDataType() == ePdfDataType_String) && (strcmp(token, "Tj") == 0))
                result.push_back(var.GetString().GetStringUtf8());
        }
    }
}

} // namespace pdf2txt

#endif //  PDF2txt_HPP
