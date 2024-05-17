bool LayerTreeHostImpl::GetScrollOffsetForLayer(int layer_id,
                                                gfx::ScrollOffset* offset) {
  LayerImpl* layer = active_tree()->FindActiveTreeLayerById(layer_id);
  if (!layer)
    return false;

  *offset = layer->CurrentScrollOffset();
  return true;
}
