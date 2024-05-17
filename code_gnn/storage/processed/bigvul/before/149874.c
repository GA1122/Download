LayerListIterator<Layer> LayerTreeHost::begin() const {
  return LayerListIterator<Layer>(root_layer_.get());
}
