bool LayerTreeHostImpl::ScrollLayerTo(int layer_id,
                                      const gfx::ScrollOffset& offset) {
  LayerImpl* layer = active_tree()->FindActiveTreeLayerById(layer_id);
  if (!layer)
    return false;

  layer->ScrollBy(
      ScrollOffsetToVector2dF(offset - layer->CurrentScrollOffset()));
  return true;
}
