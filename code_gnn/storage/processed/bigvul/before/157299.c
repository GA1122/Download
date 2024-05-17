void WebMediaPlayerImpl::RegisterContentsLayer(cc::Layer* layer) {
  DCHECK(bridge_);
  bridge_->SetContentsOpaque(opaque_);
  client_->SetCcLayer(layer);
}
