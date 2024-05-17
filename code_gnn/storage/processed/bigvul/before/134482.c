bool WebContentsViewAura::CanFocus() {
  RenderWidgetHostViewAura* view = ToRenderWidgetHostViewAura(
      web_contents_->GetRenderWidgetHostView());
  if (view != NULL && !view->IsClosing())
    return true;

  return false;
}
