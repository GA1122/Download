static Element* ElementUnderMouse(Document* document_under_mouse,
                                  const LayoutPoint& point) {
  HitTestRequest request(HitTestRequest::kReadOnly | HitTestRequest::kActive);
  HitTestResult result(request, point);
  document_under_mouse->GetLayoutView()->HitTest(result);

  Node* n = result.InnerNode();
  while (n && !n->IsElementNode())
    n = n->ParentOrShadowHostNode();
  if (n && n->IsInShadowTree())
    n = n->OwnerShadowHost();

  return ToElement(n);
}
