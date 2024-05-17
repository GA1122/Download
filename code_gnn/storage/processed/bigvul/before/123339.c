void RenderWidgetHostViewGtk::SetBounds(const gfx::Rect& rect) {
  if (IsPopup()) {
    gtk_window_move(GTK_WINDOW(gtk_widget_get_toplevel(view_.get())),
                    rect.x(), rect.y());
  }

  SetSize(rect.size());
}
