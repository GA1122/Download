void RenderWidgetHostViewAndroid::WasHidden() {
  if (host_->is_hidden())
    return;

  host_->WasHidden();
}
