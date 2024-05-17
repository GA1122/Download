bool RenderWidgetHostViewAura::CanFocus() {
  return popup_type_ == blink::WebPopupTypeNone;
}
