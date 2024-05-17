bool Document::NeedsLayoutTreeUpdateForNode(const Node& node) const {
  if (!node.CanParticipateInFlatTree())
    return false;
  if (!NeedsLayoutTreeUpdate())
    return false;
  if (!node.isConnected())
    return false;

  if (NeedsFullLayoutTreeUpdate() || node.NeedsStyleRecalc() ||
      node.NeedsStyleInvalidation())
    return true;
  for (const ContainerNode* ancestor = LayoutTreeBuilderTraversal::Parent(node);
       ancestor; ancestor = LayoutTreeBuilderTraversal::Parent(*ancestor)) {
    if (ancestor->NeedsStyleRecalc() || ancestor->NeedsStyleInvalidation() ||
        ancestor->NeedsAdjacentStyleRecalc())
      return true;
  }
  return false;
}
