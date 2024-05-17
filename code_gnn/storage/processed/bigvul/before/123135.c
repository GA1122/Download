void RenderWidgetHostViewAndroid::Show() {
  if (is_layer_attached_)
    return;

  is_layer_attached_ = true;
  if (content_view_core_)
    content_view_core_->AttachLayer(layer_);
}
