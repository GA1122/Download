Element* Document::SequentialFocusNavigationStartingPoint(
    WebFocusType type) const {
  if (focused_element_)
    return focused_element_.Get();
  if (!sequential_focus_navigation_starting_point_)
    return nullptr;
  if (!sequential_focus_navigation_starting_point_->collapsed()) {
    Node* node = sequential_focus_navigation_starting_point_->startContainer();
    DCHECK_EQ(node,
              sequential_focus_navigation_starting_point_->endContainer());
    if (node->IsElementNode())
      return ToElement(node);
    if (Element* neighbor_element = type == kWebFocusTypeForward
                                        ? ElementTraversal::Previous(*node)
                                        : ElementTraversal::Next(*node))
      return neighbor_element;
    return node->ParentOrShadowHostElement();
  }

  if (sequential_focus_navigation_starting_point_->startContainer()
          ->IsElementNode() &&
      !sequential_focus_navigation_starting_point_->startContainer()
           ->hasChildren() &&
      sequential_focus_navigation_starting_point_->startOffset() == 0)
    return ToElement(
        sequential_focus_navigation_starting_point_->startContainer());

  if (Node* next_node =
          sequential_focus_navigation_starting_point_->FirstNode()) {
    if (type == kWebFocusTypeForward)
      return ElementTraversal::Previous(*next_node);
    if (next_node->IsElementNode())
      return ToElement(next_node);
    return ElementTraversal::Next(*next_node);
  }
  return nullptr;
}
