bool RenderWidgetHostViewGtk::IsPopup() const {
  return popup_type_ != WebKit::WebPopupTypeNone;
}
