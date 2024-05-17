void LayerTreeHost::DidBeginMainFrame() {
  inside_main_frame_ = false;
  client_->DidBeginMainFrame();
}
