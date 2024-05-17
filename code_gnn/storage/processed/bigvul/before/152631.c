void HTMLFormControlElement::didMoveToNewDocument(Document& oldDocument) {
  ListedElement::didMoveToNewDocument(oldDocument);
  HTMLElement::didMoveToNewDocument(oldDocument);
}
