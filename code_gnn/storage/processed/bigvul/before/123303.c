bool RenderWidgetHostViewGtk::HasFocus() const {
  return gtk_widget_is_focus(view_.get());
}
