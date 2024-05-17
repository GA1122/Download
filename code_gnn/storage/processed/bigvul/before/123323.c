void RenderWidgetHostViewGtk::OnDestroy(GtkWidget* widget) {
  DCHECK(is_fullscreen_);
  host_->Shutdown();
}
