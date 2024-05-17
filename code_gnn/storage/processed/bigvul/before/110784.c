bool AutocompleteEditModel::DoInstant(const AutocompleteMatch& match,
                                      string16* suggested_text) {
  DCHECK(suggested_text);

  if (in_revert_)
    return false;

  InstantController* instant = controller_->GetInstant();

  if (!instant)
    return false;

  TabContentsWrapper* tab = controller_->GetTabContentsWrapper();
  if (!tab)
    return false;

  if (user_input_in_progress_ && popup_->IsOpen()) {
    return instant->Update(tab, match, view_->GetText(), UseVerbatimInstant(),
                           suggested_text);
  }

  instant->Hide();
  return false;
}
