void WebContentsViewAura::AttachTouchEditableToRenderView() {
  if (!touch_editable_)
    return;
  RenderWidgetHostViewAura* rwhva = ToRenderWidgetHostViewAura(
      web_contents_->GetRenderWidgetHostView());
  touch_editable_->AttachToView(rwhva);
}
