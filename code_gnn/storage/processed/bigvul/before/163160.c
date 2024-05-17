void HTMLIFrameElement::RemovedFrom(ContainerNode* insertion_point) {
  HTMLFrameElementBase::RemovedFrom(insertion_point);
  if (insertion_point->IsInDocumentTree() && GetDocument().IsHTMLDocument())
    ToHTMLDocument(GetDocument()).RemoveNamedItem(name_);
}
