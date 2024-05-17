void RenderWidgetHostViewGtk::OnAcceleratedCompositingStateChange() {
  bool activated = host_->is_accelerated_compositing_active();
  GtkPreserveWindow* widget = reinterpret_cast<GtkPreserveWindow*>(view_.get());

  gtk_preserve_window_delegate_resize(widget, activated);
}
