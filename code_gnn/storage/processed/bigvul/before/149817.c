void LayerTreeHost::RegisterElement(ElementId element_id,
                                    ElementListType list_type,
                                    Layer* layer) {
  if (layer->element_id()) {
    element_layers_map_[layer->element_id()] = layer;
  }

  mutator_host_->RegisterElement(element_id, list_type);
}
