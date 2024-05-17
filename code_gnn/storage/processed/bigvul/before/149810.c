bool LayerTreeHost::LayerNeedsPushPropertiesForTesting(Layer* layer) const {
  return layers_that_should_push_properties_.find(layer) !=
         layers_that_should_push_properties_.end();
}
