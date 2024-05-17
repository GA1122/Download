bool AutocompleteEditModel::AcceptKeyword() {
  DCHECK(is_keyword_hint_ && !keyword_.empty());

  autocomplete_controller_->Stop(false);
  is_keyword_hint_ = false;

  if (popup_->IsOpen())
    popup_->SetSelectedLineState(AutocompletePopupModel::KEYWORD);
  else
    StartAutocomplete(false, true);

  view_->OnTemporaryTextMaybeChanged(
      DisplayTextFromUserText(CurrentMatch().fill_into_edit),
      !has_temporary_text_);
  has_temporary_text_ = true;

  content::RecordAction(UserMetricsAction("AcceptedKeywordHint"));
  return true;
}
