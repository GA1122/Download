Document* Document::Create(Document& document) {
  Document* new_document = MakeGarbageCollected<Document>(
      DocumentInit::Create().WithContextDocument(&document).WithURL(
          BlankURL()));
  new_document->SetSecurityOrigin(document.GetMutableSecurityOrigin());
  new_document->SetContextFeatures(document.GetContextFeatures());
  return new_document;
}
