void Compositor::SetLayerTreeFrameSink(
    std::unique_ptr<cc::LayerTreeFrameSink> layer_tree_frame_sink) {
  layer_tree_frame_sink_requested_ = false;
  host_->SetLayerTreeFrameSink(std::move(layer_tree_frame_sink));
  if (context_factory_private_) {
    context_factory_private_->SetDisplayVisible(this, host_->IsVisible());
    context_factory_private_->SetDisplayColorSpace(this, blending_color_space_,
                                                   output_color_space_);
    context_factory_private_->SetDisplayColorMatrix(this,
                                                    display_color_matrix_);
  }
}
