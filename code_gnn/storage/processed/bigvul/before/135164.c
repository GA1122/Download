PassRefPtrWillBeRawPtr<Document> Document::cloneDocumentWithoutChildren()
{
    DocumentInit init(url());
    if (isXMLDocument()) {
        if (isXHTMLDocument())
            return XMLDocument::createXHTML(init.withRegistrationContext(registrationContext()));
        return XMLDocument::create(init);
    }
    return create(init);
}
