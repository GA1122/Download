void AutocompleteEditModel::StartAutocomplete(
    bool has_selected_text,
    bool prevent_inline_autocomplete) const {
  ClearPopupKeywordMode();

  bool keyword_is_selected = KeywordIsSelected();
  popup_->SetHoveredLine(AutocompletePopupModel::kNoMatch);
  autocomplete_controller_->Start(
      user_text_, GetDesiredTLD(),
      prevent_inline_autocomplete || just_deleted_text_ ||
      (has_selected_text && inline_autocomplete_text_.empty()) ||
      (paste_state_ != NONE), keyword_is_selected,
      keyword_is_selected || allow_exact_keyword_match_,
      AutocompleteInput::ALL_MATCHES);
}
