void RenderWidgetHostViewGtk::Focus() {
  gtk_widget_grab_focus(view_.get());
}
