void AutocompleteEditModel::OnSetFocus(bool control_down) {
  has_focus_ = true;
  control_key_state_ = control_down ? DOWN_WITHOUT_CHANGE : UP;

  InstantController* instant = controller_->GetInstant();
  TabContentsWrapper* tab = controller_->GetTabContentsWrapper();
  if (instant && tab)
    instant->OnAutocompleteGotFocus(tab);
}
