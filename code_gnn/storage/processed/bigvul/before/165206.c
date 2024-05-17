bool SelectTextInsteadOfDrag(const Node& node) {
  if (!node.IsTextNode())
    return false;

  if (HasEditableStyle(node))
    return true;

  for (Node& node : NodeTraversal::InclusiveAncestorsOf(node)) {
    if (node.IsHTMLElement() && ToHTMLElement(&node)->draggable())
      return false;
  }

  return node.CanStartSelection();
}
