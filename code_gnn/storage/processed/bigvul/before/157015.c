void WebMediaPlayerMS::UnregisterContentsLayer(cc::Layer* layer) {
  DCHECK(thread_checker_.CalledOnValidThread());
  client_->SetCcLayer(nullptr);
}
