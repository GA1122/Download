bool RenderWidgetHostViewGtk::IsShowing() {
  return gtk_widget_get_visible(view_.get());
}
