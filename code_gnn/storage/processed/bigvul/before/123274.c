BackingStore* RenderWidgetHostViewGtk::AllocBackingStore(
    const gfx::Size& size) {
  gint depth = gdk_visual_get_depth(gtk_widget_get_visual(view_.get()));
  return new BackingStoreGtk(host_, size,
                             ui::GetVisualFromGtkWidget(view_.get()),
                             depth);
}
