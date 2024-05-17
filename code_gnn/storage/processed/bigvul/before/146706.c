static void AssertLayoutTreeUpdated(Node& root) {
  for (Node& node : NodeTraversal::InclusiveDescendantsOf(root)) {
    if (!node.IsElementNode() && !node.IsTextNode() && !node.IsShadowRoot() &&
        !node.IsDocumentNode())
      continue;
    DCHECK(!node.NeedsStyleRecalc());
    DCHECK(!node.ChildNeedsStyleRecalc());
    DCHECK(!node.NeedsReattachLayoutTree());
    DCHECK(!node.ChildNeedsReattachLayoutTree());
    DCHECK(!node.ChildNeedsDistributionRecalc());
    DCHECK(!node.NeedsStyleInvalidation());
    DCHECK(!node.ChildNeedsStyleInvalidation());
    for (ShadowRoot* shadow_root = node.YoungestShadowRoot(); shadow_root;
         shadow_root = shadow_root->OlderShadowRoot())
      AssertLayoutTreeUpdated(*shadow_root);
  }
}
