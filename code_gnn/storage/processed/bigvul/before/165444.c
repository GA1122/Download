static void AssertLayoutTreeUpdated(Node& root) {
  Node* node = &root;
  while (node) {
    if (RuntimeEnabledFeatures::DisplayLockingEnabled() &&
        node->IsElementNode() &&
        ToElement(node)->StyleRecalcBlockedByDisplayLock()) {
      node = NodeTraversal::NextSkippingChildren(*node);
      continue;
    }

    DCHECK(!node->NeedsStyleRecalc());
    DCHECK(!node->ChildNeedsStyleRecalc());
    DCHECK(!node->NeedsReattachLayoutTree());
    DCHECK(!node->ChildNeedsReattachLayoutTree());
    DCHECK(!node->ChildNeedsDistributionRecalc());
    DCHECK(!node->NeedsStyleInvalidation());
    DCHECK(!node->ChildNeedsStyleInvalidation());
    DCHECK(!node->GetForceReattachLayoutTree());
    DCHECK((node->IsDocumentNode() || !node->GetLayoutObject() ||
            FlatTreeTraversal::Parent(*node)))
        << *node;

    if (ShadowRoot* shadow_root = node->GetShadowRoot())
      AssertLayoutTreeUpdated(*shadow_root);
    node = NodeTraversal::Next(*node);
  }
}