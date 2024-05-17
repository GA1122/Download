void Document::UpdateStyleAndLayoutForNode(const Node* node) {
  DCHECK(node);
  if (!node->InActiveDocument())
    return;

  DisplayLockUtilities::ScopedChainForcedUpdate scoped_update_forced(node);
  UpdateStyleAndLayout();
}
