void LayerTreeHost::BeginMainFrame(const BeginFrameArgs& args) {
  client_->BeginMainFrame(args);
}
