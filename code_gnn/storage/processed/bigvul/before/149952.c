gfx::ScrollOffset LayerTreeHostImpl::GetScrollOffsetForAnimation(
    ElementId element_id) const {
  if (active_tree()) {
    LayerImpl* layer = active_tree()->LayerByElementId(element_id);
    if (layer)
      return layer->ScrollOffsetForAnimation();
  }

  return gfx::ScrollOffset();
}
