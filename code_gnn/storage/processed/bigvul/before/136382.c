bool PaintArtifactCompositor::PendingLayer::CanMerge(
    const PendingLayer& guest) const {
  if (requires_own_layer || guest.requires_own_layer)
    return false;
  if (property_tree_state.Effect() != guest.property_tree_state.Effect())
    return false;
  return CanUpcastTo(guest.property_tree_state, property_tree_state);
}
