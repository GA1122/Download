static Document* ParentDocument(DocumentLoader* loader) {
  DCHECK(loader);
  DCHECK(loader->GetFrame());

  Element* owner_element = loader->GetFrame()->DeprecatedLocalOwner();
  if (!owner_element)
    return nullptr;
  return &owner_element->GetDocument();
}
