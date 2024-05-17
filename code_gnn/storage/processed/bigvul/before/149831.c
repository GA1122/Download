void LayerTreeHost::SetCompositorFrameSink(
    std::unique_ptr<CompositorFrameSink> surface) {
  TRACE_EVENT0("cc", "LayerTreeHostInProcess::SetCompositorFrameSink");
  DCHECK(surface);

  DCHECK(!new_compositor_frame_sink_);
  new_compositor_frame_sink_ = std::move(surface);
  proxy_->SetCompositorFrameSink(new_compositor_frame_sink_.get());
}
