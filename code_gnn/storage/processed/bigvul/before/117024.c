gboolean OmniboxPopupViewGtk::HandleMotion(GtkWidget* widget,
                                           GdkEventMotion* event) {
  size_t line = LineFromY(static_cast<int>(event->y));
  model_->SetHoveredLine(line);
  if (!ignore_mouse_drag_ && (event->state & GDK_BUTTON1_MASK))
    model_->SetSelectedLine(line, false, false);
  return TRUE;
}
