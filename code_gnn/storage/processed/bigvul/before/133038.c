void RenderWidgetHostViewAura::Show() {
  window_->Show();
  WasShown();
}
