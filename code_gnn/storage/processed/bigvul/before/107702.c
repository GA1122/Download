gboolean StatusBubbleGtk::HandleEnterNotify(GtkWidget* sender,
                                            GdkEventCrossing* event) {
  ignore_next_left_content_ = true;
  MouseMoved(gfx::Point(event->x_root, event->y_root), false);
  return FALSE;
}
