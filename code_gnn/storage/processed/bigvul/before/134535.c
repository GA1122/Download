void WebContentsViewAura::PrepareOverscrollNavigationOverlay() {
  OverscrollWindowDelegate* delegate = static_cast<OverscrollWindowDelegate*>(
      overscroll_window_->delegate());
  overscroll_window_->SchedulePaintInRect(
      gfx::Rect(overscroll_window_->bounds().size()));
  overscroll_window_->SetBounds(gfx::Rect(window_->bounds().size()));
  overscroll_window_->SetTransform(gfx::Transform());
  navigation_overlay_->SetOverlayWindow(overscroll_window_.Pass(),
                                        delegate);
  navigation_overlay_->StartObservingView(ToRenderWidgetHostViewAura(
      web_contents_->GetRenderWidgetHostView()));
}
