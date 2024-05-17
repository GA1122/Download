PassRefPtrWillBeRawPtr<DocumentParser> Document::createParser()
{
    if (isHTMLDocument()) {
        bool reportErrors = InspectorInstrumentation::collectingHTMLParseErrors(this);
        return HTMLDocumentParser::create(toHTMLDocument(*this), reportErrors, m_parserSyncPolicy);
    }
    return XMLDocumentParser::create(*this, view());
}
