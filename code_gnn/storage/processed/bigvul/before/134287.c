bool OmniboxViewViews::OnMousePressed(const ui::MouseEvent& event) {
  select_all_on_mouse_release_ =
      (event.IsOnlyLeftMouseButton() || event.IsOnlyRightMouseButton()) &&
      (!HasFocus() || (model()->focus_state() == OMNIBOX_FOCUS_INVISIBLE));
  if (select_all_on_mouse_release_) {
    model()->SetCaretVisibility(true);

    saved_selection_for_focus_change_ = gfx::Range::InvalidRange();
  }
  return views::Textfield::OnMousePressed(event);
}
