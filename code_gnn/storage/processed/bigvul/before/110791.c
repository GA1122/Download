void AutocompleteEditModel::InfoForCurrentSelection(
    AutocompleteMatch* match,
    GURL* alternate_nav_url) const {
  DCHECK(match != NULL);
  const AutocompleteResult& result = this->result();
  if (!autocomplete_controller_->done()) {
    if (result.empty())
      return;
    *match = *result.default_match();
  } else {
    CHECK(popup_->IsOpen());
    CHECK(!result.empty());
    CHECK(popup_->selected_line() < result.size());
    *match = result.match_at(popup_->selected_line());
  }
  if (alternate_nav_url && popup_->manually_selected_match().empty())
    *alternate_nav_url = result.alternate_nav_url();
}
