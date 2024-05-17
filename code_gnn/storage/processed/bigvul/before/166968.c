void CSSStyleSheet::DidMutateRules() {
  DCHECK(contents_->IsMutable());
  DCHECK_LE(contents_->ClientSize(), 1u);

  Document* owner = OwnerDocument();
  if (owner && ownerNode() && ownerNode()->isConnected()) {
    owner->GetStyleEngine().SetNeedsActiveStyleUpdate(
        ownerNode()->GetTreeScope());
    if (StyleResolver* resolver = owner->GetStyleEngine().Resolver())
      resolver->InvalidateMatchedPropertiesCache();
  } else if (!constructed_tree_scopes_.IsEmpty()) {
    for (auto tree_scope : constructed_tree_scopes_) {
      tree_scope->GetDocument().GetStyleEngine().SetNeedsActiveStyleUpdate(
          *tree_scope);
      if (StyleResolver* resolver =
              tree_scope->GetDocument().GetStyleEngine().Resolver())
        resolver->InvalidateMatchedPropertiesCache();
    }
  }
}
