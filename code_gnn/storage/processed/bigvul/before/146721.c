Document* Document::Create(const Document& document) {
  Document* new_document =
      new Document(DocumentInit::Create()
                       .WithContextDocument(const_cast<Document*>(&document))
                       .WithURL(BlankURL()));
  new_document->SetSecurityOrigin(document.GetSecurityOrigin());
  new_document->SetContextFeatures(document.GetContextFeatures());
  return new_document;
}
