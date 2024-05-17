Document& Document::ensureTemplateDocument()
{
    if (const Document* document = templateDocument())
        return *const_cast<Document*>(document);

    if (isHTMLDocument()) {
        DocumentInit init = DocumentInit::fromContext(contextDocument(), blankURL())
            .withRegistrationContext(registrationContext());
        m_templateDocument = HTMLDocument::create(init);
    } else {
        m_templateDocument = Document::create(DocumentInit(blankURL()));
    }

    m_templateDocument->setTemplateDocumentHost(this);  

    return *m_templateDocument.get();
}
