void WebContentsImpl::UpdateTitleForEntry(NavigationEntry* entry,
                                          const base::string16& title) {
  base::string16 final_title;
  bool explicit_set;
  if (entry && entry->GetURL().SchemeIsFile() && title.empty()) {
    final_title = base::UTF8ToUTF16(entry->GetURL().ExtractFileName());
    explicit_set = false;   
  } else {
    base::TrimWhitespace(title, base::TRIM_ALL, &final_title);
    explicit_set = true;
  }

  if (entry) {
    if (final_title == entry->GetTitle())
      return;   

    entry->SetTitle(final_title);
  } else {
    if (page_title_when_no_navigation_entry_ == final_title)
      return;   

    page_title_when_no_navigation_entry_ = final_title;
  }

  view_->SetPageTitle(final_title);

  for (auto& observer : observers_)
    observer.TitleWasSet(entry, explicit_set);

  if (entry == controller_.GetEntryAtOffset(0))
    NotifyNavigationStateChanged(INVALIDATE_TYPE_TITLE);
}
