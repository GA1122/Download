bool WebContentsImpl::UpdateTitleForEntry(NavigationEntryImpl* entry,
                                          const string16& title) {
  string16 final_title;
  bool explicit_set;
  if (entry && entry->GetURL().SchemeIsFile() && title.empty()) {
    final_title = UTF8ToUTF16(entry->GetURL().ExtractFileName());
    explicit_set = false;   
  } else {
    TrimWhitespace(title, TRIM_ALL, &final_title);
    explicit_set = true;
  }

  if (entry) {
    if (final_title == entry->GetTitle())
      return false;   

    entry->SetTitle(final_title);
  } else {
    if (page_title_when_no_navigation_entry_ == final_title)
      return false;   

    page_title_when_no_navigation_entry_ = final_title;
  }

  view_->SetPageTitle(final_title);

  std::pair<NavigationEntry*, bool> details =
      std::make_pair(entry, explicit_set);

  NotificationService::current()->Notify(
      NOTIFICATION_WEB_CONTENTS_TITLE_UPDATED,
      Source<WebContents>(this),
      Details<std::pair<NavigationEntry*, bool> >(&details));

  return true;
}
