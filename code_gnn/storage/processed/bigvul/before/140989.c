Document* Document::CloneDocumentWithoutChildren() const {
  DocumentInit init = DocumentInit::Create()
                          .WithContextDocument(ContextDocument())
                          .WithURL(Url());
  if (IsXMLDocument()) {
    if (IsXHTMLDocument())
      return XMLDocument::CreateXHTML(
          init.WithRegistrationContext(RegistrationContext()));
    return XMLDocument::Create(init);
  }
  return Create(init);
}
