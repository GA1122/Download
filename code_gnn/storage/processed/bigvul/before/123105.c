void RenderWidgetHostViewAndroid::Hide() {
  if (!is_layer_attached_)
    return;

  is_layer_attached_ = false;
  if (content_view_core_)
    content_view_core_->RemoveLayer(layer_);
}
