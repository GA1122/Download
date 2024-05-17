void Document::UpdateHoverActiveState(const HitTestRequest& request,
                                      Element* inner_element) {
  DCHECK(!request.ReadOnly());

  if (request.Active() && frame_)
    frame_->GetEventHandler().NotifyElementActivated();

  Element* inner_element_in_document = inner_element;

  while (inner_element_in_document &&
         inner_element_in_document->GetDocument() != this) {
    inner_element_in_document->GetDocument().UpdateHoverActiveState(
        request, inner_element_in_document);
    inner_element_in_document =
        inner_element_in_document->GetDocument().LocalOwner();
  }

  UpdateDistributionForFlatTreeTraversal();

  UpdateActiveState(request, inner_element_in_document);
  UpdateHoverState(request, inner_element_in_document);
}
