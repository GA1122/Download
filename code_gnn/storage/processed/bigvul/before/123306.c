void RenderWidgetHostViewGtk::InitAsChild(
    gfx::NativeView parent_view) {
  DoSharedInit();
  gtk_widget_show(view_.get());
}
