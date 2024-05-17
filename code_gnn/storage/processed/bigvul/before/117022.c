gboolean OmniboxPopupViewGtk::HandleButtonRelease(GtkWidget* widget,
                                                  GdkEventButton* event) {
  if (ignore_mouse_drag_) {
    ignore_mouse_drag_ = false;
    return TRUE;
  }

  size_t line = LineFromY(static_cast<int>(event->y));
  switch (event->button) {
    case 1:   
      AcceptLine(line, CURRENT_TAB);
      break;
    case 2:   
      AcceptLine(line, NEW_BACKGROUND_TAB);
      break;
    default:
      break;
  }
  return TRUE;
}
