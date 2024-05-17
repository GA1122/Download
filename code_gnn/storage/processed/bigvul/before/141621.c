void AXTree::DestroySubtree(AXNode* node,
                            AXTreeUpdateState* update_state) {
  DCHECK(update_state);
  for (AXTreeObserver& observer : observers_) {
    if (!update_state->IsChangedNode(node))
      observer.OnSubtreeWillBeDeleted(this, node);
    else
      observer.OnSubtreeWillBeReparented(this, node);
  }
  DestroyNodeAndSubtree(node, update_state);
}
