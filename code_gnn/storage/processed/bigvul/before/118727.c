PassRefPtr<Document> HTMLDocument::cloneDocumentWithoutChildren()
{
    return create(DocumentInit(url()).withRegistrationContext(registrationContext()));
}
