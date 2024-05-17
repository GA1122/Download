gboolean AutocompletePopupViewGtk::HandleButtonPress(GtkWidget* widget,
                                                     GdkEventButton* event) {
  ignore_mouse_drag_ = false;
  size_t line = LineFromY(static_cast<int>(event->y));
  model_->SetHoveredLine(line);
  if (event->button == 1)
    model_->SetSelectedLine(line, false);
  return TRUE;
}
