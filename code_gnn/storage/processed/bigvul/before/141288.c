void Document::UpdateStyleAndLayoutTreeForNode(const Node* node) {
  DCHECK(node);
  if (!NeedsLayoutTreeUpdateForNode(*node))
    return;

  DisplayLockUtilities::ScopedChainForcedUpdate scoped_update_forced(node);
  UpdateStyleAndLayoutTree();
}
