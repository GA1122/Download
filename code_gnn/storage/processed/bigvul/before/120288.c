LayerTreeHost::~LayerTreeHost() {
  TRACE_EVENT0("cc", "LayerTreeHost::~LayerTreeHost");

  overhang_ui_resource_.reset();

  if (root_layer_.get())
    root_layer_->SetLayerTreeHost(NULL);

  if (proxy_) {
    DCHECK(proxy_->IsMainThread());
    proxy_->Stop();
  }

  RegisterViewportLayers(NULL, NULL, NULL);

  if (root_layer_.get()) {
    root_layer_ = NULL;
  }
}
