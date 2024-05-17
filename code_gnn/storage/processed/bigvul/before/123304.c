void RenderWidgetHostViewGtk::Hide() {
  gtk_widget_hide(view_.get());
}
