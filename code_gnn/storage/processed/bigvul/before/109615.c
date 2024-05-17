PassRefPtr<DocumentParser> Document::createParser()
{
    if (isHTMLDocument()) {
        bool reportErrors = InspectorInstrumentation::collectingHTMLParseErrors(this->page());
        return HTMLDocumentParser::create(toHTMLDocument(this), reportErrors);
    }
    return XMLDocumentParser::create(this, view());
}
