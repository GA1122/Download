void Document::RemoveFocusedElementOfSubtree(Node& node,
                                             bool among_children_only) {
  if (!focused_element_)
    return;

  if (!node.isConnected())
    return;
  bool contains =
      node.IsShadowIncludingInclusiveAncestorOf(focused_element_.Get());
  if (contains && (focused_element_ != &node || !among_children_only))
    ClearFocusedElement();
}
