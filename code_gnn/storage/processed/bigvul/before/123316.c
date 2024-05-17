bool RenderWidgetHostViewGtk::NeedsInputGrab() {
  return popup_type_ == WebKit::WebPopupTypeSelect;
}
