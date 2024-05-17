gfx::ScrollOffset LayerTreeHost::GetScrollOffsetForAnimation(
    ElementId element_id) const {
  Layer* layer = LayerByElementId(element_id);
  DCHECK(layer);
  return layer->ScrollOffsetForAnimation();
}
