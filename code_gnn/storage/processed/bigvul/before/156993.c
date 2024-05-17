void WebMediaPlayerMS::RegisterContentsLayer(cc::Layer* layer) {
  DCHECK(thread_checker_.CalledOnValidThread());
  DCHECK(bridge_);

  bridge_->SetContentsOpaque(opaque_);
  client_->SetCcLayer(layer);
}
