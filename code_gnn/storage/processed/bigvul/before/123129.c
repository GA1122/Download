void RenderWidgetHostViewAndroid::SetContentViewCore(
    ContentViewCoreImpl* content_view_core) {
  if (content_view_core_ && is_layer_attached_)
    content_view_core_->RemoveLayer(layer_);

  content_view_core_ = content_view_core;
  if (content_view_core_ && is_layer_attached_)
    content_view_core_->AttachLayer(layer_);
}
