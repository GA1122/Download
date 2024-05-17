void LayerTreeHost::WillCommit() {
  swap_promise_manager_.WillCommit();
  client_->WillCommit();
}
