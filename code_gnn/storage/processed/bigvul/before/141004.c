Range* Document::CreateRangeAdjustedToTreeScope(const TreeScope& tree_scope,
                                                const Position& position) {
  DCHECK(position.IsNotNull());
  Node* const anchor_node = position.AnchorNode();
  if (anchor_node->GetTreeScope() == tree_scope)
    return Range::Create(tree_scope.GetDocument(), position, position);
  Node* const shadow_host = tree_scope.AncestorInThisScope(anchor_node);
  return Range::Create(tree_scope.GetDocument(),
                       Position::BeforeNode(*shadow_host),
                       Position::BeforeNode(*shadow_host));
}
