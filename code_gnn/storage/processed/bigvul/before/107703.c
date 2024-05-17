gboolean StatusBubbleGtk::HandleMotionNotify(GtkWidget* sender,
                                             GdkEventMotion* event) {
  MouseMoved(gfx::Point(event->x_root, event->y_root), false);
  return FALSE;
}
