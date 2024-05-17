void WebMediaPlayerImpl::UnregisterContentsLayer(cc::Layer* layer) {
  client_->SetCcLayer(nullptr);
}
