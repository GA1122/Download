static Node* NonBoundaryShadowTreeRootNode(const Position& position) {
  return position.AnchorNode() && !position.AnchorNode()->IsShadowRoot()
             ? position.AnchorNode()->NonBoundaryShadowTreeRootNode()
             : nullptr;
}
