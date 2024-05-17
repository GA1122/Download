bool RenderWidgetHostViewAndroid::HasValidFrame() const {
  return texture_id_in_layer_ != 0 &&
      content_view_core_ &&
      !texture_size_in_layer_.IsEmpty() &&
      texture_size_in_layer_ == content_view_core_->GetBounds().size();
}
