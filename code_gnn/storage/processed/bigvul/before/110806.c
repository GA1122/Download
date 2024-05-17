void AutocompleteEditModel::PasteAndGo() {
  view_->RevertAll();
  view_->OpenMatch(paste_and_go_match_, CURRENT_TAB,
      paste_and_go_alternate_nav_url_, AutocompletePopupModel::kNoMatch);
}
