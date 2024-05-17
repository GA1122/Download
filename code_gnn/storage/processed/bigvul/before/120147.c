void Layer::SavePaintProperties() {
  DCHECK(layer_tree_host_);

  paint_properties_.bounds = bounds_;
  paint_properties_.source_frame_number =
      layer_tree_host_->source_frame_number();
}
