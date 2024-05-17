void RenderWidgetHostViewAura::OnDidNavigateMainFrameToNewPage() {
  ui::GestureRecognizer::Get()->CancelActiveTouches(window_);
 }
