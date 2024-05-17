Node::InsertionNotificationRequest HTMLIFrameElement::InsertedInto(
    ContainerNode* insertion_point) {
  InsertionNotificationRequest result =
      HTMLFrameElementBase::InsertedInto(insertion_point);
  if (insertion_point->IsInDocumentTree() && GetDocument().IsHTMLDocument())
    ToHTMLDocument(GetDocument()).AddNamedItem(name_);
  LogAddElementIfIsolatedWorldAndInDocument("iframe", srcAttr);
  return result;
}
