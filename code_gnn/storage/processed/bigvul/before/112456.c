Document* Document::ensureTemplateDocument()
{
    if (const Document* document = templateDocument())
        return const_cast<Document*>(document);

    if (isHTMLDocument())
        m_templateDocument = HTMLDocument::create(0, blankURL());
    else
        m_templateDocument = Document::create(0, blankURL());

    m_templateDocument->setTemplateDocumentHost(this);  

    return m_templateDocument.get();
}
