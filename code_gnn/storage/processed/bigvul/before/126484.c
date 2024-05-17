bool TabStripGtk::CompleteDrop(const guchar* data, bool is_plain_text) {
  if (!drop_info_.get())
    return false;

  const int drop_index = drop_info_->drop_index;
  const bool drop_before = drop_info_->drop_before;

  drop_info_.reset();

  hover_tab_selector_.CancelTabTransition();

  GURL url;
  if (is_plain_text) {
    AutocompleteMatch match;
    AutocompleteClassifierFactory::GetForProfile(model_->profile())->Classify(
        UTF8ToUTF16(reinterpret_cast<const char*>(data)), string16(),
        false, false, &match, NULL);
    url = match.destination_url;
  } else {
    std::string url_string(reinterpret_cast<const char*>(data));
    url = GURL(url_string.substr(0, url_string.find_first_of('\n')));
  }
  if (!url.is_valid())
    return false;

  chrome::NavigateParams params(window()->browser(), url,
                                content::PAGE_TRANSITION_LINK);
  params.tabstrip_index = drop_index;

  if (drop_before) {
    params.disposition = NEW_FOREGROUND_TAB;
  } else {
    params.disposition = CURRENT_TAB;
    params.source_contents = model_->GetTabContentsAt(drop_index);
  }

  chrome::Navigate(&params);

  return true;
}
