void WebContentsViewAura::RenderViewSwappedIn(RenderViewHost* host) {
  if (navigation_overlay_.get() && navigation_overlay_->has_window()) {
    navigation_overlay_->StartObservingView(
        ToRenderWidgetHostViewAura(host->GetView()));
  }
  AttachTouchEditableToRenderView();
}
