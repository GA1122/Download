HeapVector<VRLayer> VRDisplay::getLayers() {
  HeapVector<VRLayer> layers;

  if (is_presenting_) {
    layers.push_back(layer_);
  }

  return layers;
}
