bool OmniboxViewViews::OnMouseDragged(const ui::MouseEvent& event) {
  if (HasTextBeingDragged())
    CloseOmniboxPopup();

  bool handled = views::Textfield::OnMouseDragged(event);

  if (HasSelection() ||
      ExceededDragThreshold(event.location() - GetLastClickLocation())) {
    select_all_on_mouse_release_ = false;
  }

  return handled;
}
