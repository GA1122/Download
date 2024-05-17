void LayerTreeHost::UnregisterElement(ElementId element_id,
                                      ElementListType list_type,
                                      Layer* layer) {
  mutator_host_->UnregisterElement(element_id, list_type);

  if (layer->element_id()) {
    element_layers_map_.erase(layer->element_id());
  }
}
