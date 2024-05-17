void Compositor::RequestNewLayerTreeFrameSink() {
  DCHECK(!layer_tree_frame_sink_requested_);
  layer_tree_frame_sink_requested_ = true;
  if (widget_valid_) {
    context_factory_->CreateLayerTreeFrameSink(
        context_creation_weak_ptr_factory_.GetWeakPtr());
  }
}
