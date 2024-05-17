void RenderViewImpl::SetBackgroundOpaqueForWidget(bool opaque) {
  if (!frame_widget_)
    return;

  if (opaque) {
    frame_widget_->ClearBaseBackgroundColorOverride();
    frame_widget_->ClearBackgroundColorOverride();
  } else {
    frame_widget_->SetBaseBackgroundColorOverride(SK_ColorTRANSPARENT);
    frame_widget_->SetBackgroundColorOverride(SK_ColorTRANSPARENT);
  }
}
