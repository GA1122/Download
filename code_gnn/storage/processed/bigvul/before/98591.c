void DraggedTabGtk::Detach() {
  attached_ = false;
  ResizeContainer();

  if (gtk_util::IsScreenComposited())
    gdk_window_set_opacity(container_->window, kTransparentAlpha);
}
