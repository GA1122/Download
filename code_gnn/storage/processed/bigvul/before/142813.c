bool HTMLMediaElement::HavePotentialSourceChild() {
  HTMLSourceElement* current_source_node = current_source_node_;
  Node* next_node = next_child_node_to_consider_;

  KURL next_url = SelectNextSourceChild(nullptr, kDoNothing);

  current_source_node_ = current_source_node;
  next_child_node_to_consider_ = next_node;

  return next_url.IsValid();
}
