void RenderWidgetHostViewGuest::WasShown() {
  if (!is_hidden_)
    return;

  is_hidden_ = false;
  host_->WasShown();
}
