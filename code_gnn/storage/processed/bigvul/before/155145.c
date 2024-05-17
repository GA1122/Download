bool OmniboxViewViews::OnMouseDragged(const ui::MouseEvent& event) {
  if (filter_drag_events_for_unelision_ &&
      !ExceededDragThreshold(event.root_location() -
                             GetLastClickRootLocation())) {
    return true;
  }

  if (HasTextBeingDragged())
    CloseOmniboxPopup();

  bool handled = views::Textfield::OnMouseDragged(event);

  if (HasSelection() || ExceededDragThreshold(event.root_location() -
                                              GetLastClickRootLocation())) {
    select_all_on_mouse_release_ = false;
  }

  return handled;
}
