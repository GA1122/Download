Document* HTMLLinkElement::import() const {
  if (LinkImport* link = GetLinkImport())
    return link->ImportedDocument();
  return nullptr;
}
