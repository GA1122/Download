void OmniboxPopupViewGtk::GetVisibleMatchForInput(
    size_t index,
    const AutocompleteMatch** match,
    bool* is_selected_keyword) {
  const AutocompleteResult& result = model_->result();

  if (result.match_at(index).associated_keyword.get() &&
      model_->selected_line() == index &&
      model_->selected_line_state() == AutocompletePopupModel::KEYWORD) {
    *match = result.match_at(index).associated_keyword.get();
    *is_selected_keyword = true;
    return;
  }

  *match = &result.match_at(index);
  *is_selected_keyword = false;
}
