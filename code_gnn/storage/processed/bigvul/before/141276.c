void Document::UpdateActiveState(const HitTestRequest& request,
                                 Element* inner_element_in_document) {
  Element* old_active_element = GetActiveElement();
  if (old_active_element && !request.Active()) {
    for (Element* element = old_active_element; element;
         element = FlatTreeTraversal::ParentElement(*element)) {
      element->SetActive(false);
      user_action_elements_.SetInActiveChain(element, false);
    }
    SetActiveElement(nullptr);
  } else {
    Element* new_active_element = inner_element_in_document;
    if (!old_active_element && new_active_element &&
        !new_active_element->IsDisabledFormControl() && request.Active() &&
        !request.TouchMove()) {
      for (Element* element = new_active_element; element;
           element = FlatTreeTraversal::ParentElement(*element)) {
        user_action_elements_.SetInActiveChain(element, true);
      }
      SetActiveElement(new_active_element);
    }
  }

  bool allow_active_changes = !old_active_element && GetActiveElement();
  if (!allow_active_changes)
    return;

  bool must_be_in_active_chain = request.Active() && request.Move();

  Element* new_element = SkipDisplayNoneAncestors(inner_element_in_document);

  for (Element* curr = new_element; curr;
       curr = FlatTreeTraversal::ParentElement(*curr)) {
    if (!must_be_in_active_chain || curr->InActiveChain())
      curr->SetActive(true);
  }
}
