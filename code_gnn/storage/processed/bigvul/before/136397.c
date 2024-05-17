void PaintArtifactCompositor::RemoveChildLayers() {
  cc::LayerTreeHost* host = root_layer_->layer_tree_host();
  if (!host)
    return;
  for (auto child : root_layer_->children()) {
    host->UnregisterElement(child->element_id(), cc::ElementListType::ACTIVE);
  }
  root_layer_->RemoveAllChildren();
  if (extra_data_for_testing_enabled_) {
    extra_data_for_testing_->content_layers.clear();
    extra_data_for_testing_->synthesized_clip_layers.clear();
    extra_data_for_testing_->scroll_hit_test_layers.clear();
  }
}
