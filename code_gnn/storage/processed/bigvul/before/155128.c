bool OmniboxViewViews::MaybeUnfocusTabButton() {
  if (SelectedSuggestionHasTabMatch() &&
      model()->popup_model()->selected_line_state() ==
          OmniboxPopupModel::BUTTON_FOCUSED) {
    model()->popup_model()->SetSelectedLineState(OmniboxPopupModel::NORMAL);
    return true;
  }
  return false;
}
