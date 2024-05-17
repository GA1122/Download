void DraggedTabGtk::Attach(int selected_width) {
  attached_ = true;
  Resize(selected_width);

  if (gtk_util::IsScreenComposited())
    gdk_window_set_opacity(container_->window, kOpaqueAlpha);
}
