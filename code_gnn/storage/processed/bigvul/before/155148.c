bool OmniboxViewViews::OnMousePressed(const ui::MouseEvent& event) {
  if (model()->popup_model() &&   
      model()->popup_model()->selected_line_state() ==
          OmniboxPopupModel::BUTTON_FOCUSED) {
    model()->popup_model()->SetSelectedLineState(OmniboxPopupModel::NORMAL);
  }
  is_mouse_pressed_ = true;

  select_all_on_mouse_release_ =
      (event.IsOnlyLeftMouseButton() || event.IsOnlyRightMouseButton()) &&
      (!HasFocus() || (model()->focus_state() == OMNIBOX_FOCUS_INVISIBLE));
  if (select_all_on_mouse_release_) {
    model()->SetCaretVisibility(true);

    saved_selection_for_focus_change_ = gfx::Range::InvalidRange();
  }

  bool handled = views::Textfield::OnMousePressed(event);

  if (!select_all_on_mouse_release_ &&
      UnapplySteadyStateElisions(UnelisionGesture::OTHER)) {
    TextChanged();
    filter_drag_events_for_unelision_ = true;
  }

  return handled;
}
