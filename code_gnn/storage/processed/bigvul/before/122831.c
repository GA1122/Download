void CompositorSwapClient::OnLostContext() {
  factory_->OnLostContext(compositor_);
}
