bool RenderWidgetHostViewAura::NeedsInputGrab() {
  return popup_type_ == blink::WebPopupTypeSelect;
}
