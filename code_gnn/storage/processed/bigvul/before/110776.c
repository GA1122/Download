void AutocompleteEditModel::ClearKeyword(const string16& visible_text) {
  autocomplete_controller_->Stop(false);
  ClearPopupKeywordMode();

  const string16 window_text(keyword_ + visible_text);

  if (just_deleted_text_ || !visible_text.empty() || !popup_->IsOpen()) {
    view_->OnBeforePossibleChange();
    view_->SetWindowTextAndCaretPos(window_text.c_str(), keyword_.length(),
        false, false);
    keyword_.clear();
    is_keyword_hint_ = false;
    view_->OnAfterPossibleChange();
    just_deleted_text_ = true;   
  } else {
    is_keyword_hint_ = true;
    view_->SetWindowTextAndCaretPos(window_text.c_str(), keyword_.length(),
        false, true);
  }
}
