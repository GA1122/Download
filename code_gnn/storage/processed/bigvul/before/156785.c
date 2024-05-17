static void AssertLayoutTreeUpdated(Node& root) {
  for (Node& node : NodeTraversal::InclusiveDescendantsOf(root)) {
    DCHECK(!node.NeedsStyleRecalc());
    DCHECK(!node.ChildNeedsStyleRecalc());
    DCHECK(!node.NeedsReattachLayoutTree());
    DCHECK(!node.ChildNeedsReattachLayoutTree());
    DCHECK(!node.ChildNeedsDistributionRecalc());
    DCHECK(!node.NeedsStyleInvalidation());
    DCHECK(!node.ChildNeedsStyleInvalidation());
    DCHECK((node.IsDocumentNode() || !node.GetLayoutObject() ||
            FlatTreeTraversal::Parent(node)))
        << node;

    if (ShadowRoot* shadow_root = node.GetShadowRoot())
      AssertLayoutTreeUpdated(*shadow_root);
  }
}
