bool RenderWidgetHostViewAndroid::HasFocus() const {
  if (!content_view_core_)
    return false;   

  return content_view_core_->HasFocus();
}
