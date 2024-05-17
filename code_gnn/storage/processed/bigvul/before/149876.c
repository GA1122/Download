LayerListReverseIterator<Layer> LayerTreeHost::rbegin() {
  return LayerListReverseIterator<Layer>(root_layer_.get());
}
