void RenderWidgetHostViewGtk::WasHidden() {
  if (is_hidden_)
    return;

  is_hidden_ = true;

  host_->WasHidden();
}
