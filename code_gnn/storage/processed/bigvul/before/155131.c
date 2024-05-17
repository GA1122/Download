bool OmniboxViewViews::OnAfterPossibleChange(bool allow_keyword_ui_change) {
  State new_state;
  GetState(&new_state);
  OmniboxView::StateChanges state_changes =
      GetStateChanges(state_before_change_, new_state);

  state_changes.text_differs =
      state_changes.text_differs ||
      (ime_composing_before_change_ != IsIMEComposing());

  bool something_changed = model()->OnAfterPossibleChange(
      state_changes, allow_keyword_ui_change && !IsIMEComposing());

  if (state_changes.selection_differs && !is_mouse_pressed_ &&
      UnapplySteadyStateElisions(UnelisionGesture::OTHER)) {
    something_changed = true;
    state_changes.text_differs = true;
  }

  if (something_changed &&
      (state_changes.text_differs || state_changes.keyword_differs))
    TextChanged();
  else if (state_changes.selection_differs)
    EmphasizeURLComponents();

  return something_changed;
}
