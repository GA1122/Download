void Document::UpdateStyleAndLayoutTreeForNode(const Node* node) {
  DCHECK(node);
  if (!NeedsLayoutTreeUpdateForNode(*node))
    return;
  UpdateStyleAndLayoutTree();
}
