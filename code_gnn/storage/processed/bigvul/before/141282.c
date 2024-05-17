void Document::UpdateHoverState(const HitTestRequest& request,
                                Element* inner_element_in_document) {
  Element* old_hover_element = HoverElement();

  Element* new_hover_element =
      SkipDisplayNoneAncestors(inner_element_in_document);

  SetHoverElement(new_hover_element);

  if (old_hover_element == new_hover_element)
    return;

  Node* ancestor_element = nullptr;
  if (old_hover_element && old_hover_element->isConnected() &&
      new_hover_element) {
    Node* ancestor = FlatTreeTraversal::CommonAncestor(*old_hover_element,
                                                       *new_hover_element);
    if (ancestor && ancestor->IsElementNode())
      ancestor_element = ToElement(ancestor);
  }

  HeapVector<Member<Element>, 32> elements_to_remove_from_chain;
  HeapVector<Member<Element>, 32> elements_to_add_to_hover_chain;

  if (old_hover_element && old_hover_element->isConnected()) {
    for (Element* curr = old_hover_element; curr && curr != ancestor_element;
         curr = FlatTreeTraversal::ParentElement(*curr)) {
      elements_to_remove_from_chain.push_back(curr);
    }
  }

  for (Element* curr = new_hover_element; curr;
       curr = FlatTreeTraversal::ParentElement(*curr)) {
    elements_to_add_to_hover_chain.push_back(curr);
  }

  for (Element* element : elements_to_remove_from_chain)
    element->SetHovered(false);

  bool saw_common_ancestor = false;
  for (Element* element : elements_to_add_to_hover_chain) {
    if (element == ancestor_element)
      saw_common_ancestor = true;
    if (!saw_common_ancestor || element == hover_element_)
      element->SetHovered(true);
  }
}
