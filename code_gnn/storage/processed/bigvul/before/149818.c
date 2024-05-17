 void LayerTreeHost::RegisterLayer(Layer* layer) {
   DCHECK(!LayerById(layer->id()));
   DCHECK(!in_paint_layer_contents_);
  layer_id_map_[layer->id()] = layer;
  if (layer->element_id()) {
    mutator_host_->RegisterElement(layer->element_id(),
                                   ElementListType::ACTIVE);
  }
}
