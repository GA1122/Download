std::string LayerTreeHostImpl::LayerTreeAsJson() const {
  std::string str;
  if (active_tree_->root_layer_for_testing()) {
    std::unique_ptr<base::Value> json(
        active_tree_->root_layer_for_testing()->LayerTreeAsJson());
    base::JSONWriter::WriteWithOptions(
        *json, base::JSONWriter::OPTIONS_PRETTY_PRINT, &str);
  }
  return str;
}
