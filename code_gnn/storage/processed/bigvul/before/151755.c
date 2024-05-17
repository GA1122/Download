void Browser::TabReplacedAt(TabStripModel* tab_strip_model,
                            WebContents* old_contents,
                            WebContents* new_contents,
                            int index) {
  TabDetachedAtImpl(old_contents, index, DETACH_TYPE_REPLACE);
  exclusive_access_manager_->OnTabClosing(old_contents);
  SessionService* session_service =
      SessionServiceFactory::GetForProfile(profile_);
  if (session_service)
    session_service->TabClosing(old_contents);
  TabInsertedAt(tab_strip_model, new_contents, index,
                (index == tab_strip_model_->active_index()));

  if (!new_contents->GetController().IsInitialBlankNavigation()) {
    int entry_count = new_contents->GetController().GetEntryCount();
    new_contents->GetController().NotifyEntryChanged(
        new_contents->GetController().GetEntryAtIndex(entry_count - 1));
  }

  if (session_service) {
    session_service->TabRestored(new_contents,
                                 tab_strip_model_->IsTabPinned(index));
  }
}
