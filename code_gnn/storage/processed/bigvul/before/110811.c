void AutocompleteEditModel::RevertTemporaryText(bool revert_popup) {
  just_deleted_text_ = false;
  has_temporary_text_ = false;
  if (revert_popup)
    popup_->ResetToDefaultMatch();
  view_->OnRevertTemporaryText();
}
