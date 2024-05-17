ContainerNode* RootEditableElementOrTreeScopeRootNodeOf(
    const Position& position) {
  Element* selection_root = RootEditableElementOf(position);
  if (selection_root)
    return selection_root;

  Node* const node = position.ComputeContainerNode();
  return node ? &node->GetTreeScope().RootNode() : 0;
}
