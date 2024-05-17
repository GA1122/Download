gboolean BrowserWindowGtk::OnExposeDrawInfobarBits(GtkWidget* sender,
                                                   GdkEventExpose* expose) {
  TRACE_EVENT0("ui::gtk", "BrowserWindowGtk::OnExposeDrawInfobarBits");
  infobar_container_->PaintInfobarBitsOn(sender, expose, NULL);

  return FALSE;
}
