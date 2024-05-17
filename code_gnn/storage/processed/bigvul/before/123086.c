void RenderWidgetHostViewAndroid::Destroy() {
  if (content_view_core_) {
    content_view_core_->RemoveLayer(layer_);
    content_view_core_ = NULL;
  }

  host_ = NULL;

  delete this;
}
