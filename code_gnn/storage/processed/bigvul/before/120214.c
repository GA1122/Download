scoped_ptr<OutputSurface> LayerTreeHost::CreateOutputSurface() {
  return client_->CreateOutputSurface(num_failed_recreate_attempts_ >= 4);
}
