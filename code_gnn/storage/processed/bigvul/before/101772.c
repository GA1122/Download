void Browser::ProcessPendingUIUpdates() {
#ifndef NDEBUG
  for (UpdateMap::const_iterator i = scheduled_updates_.begin();
       i != scheduled_updates_.end(); ++i) {
    bool found = false;
    for (int tab = 0; tab < tab_count(); tab++) {
      if (GetTabContentsAt(tab) == i->first) {
        found = true;
        break;
      }
    }
    DCHECK(found);
  }
#endif

  chrome_updater_factory_.RevokeAll();

  for (UpdateMap::const_iterator i = scheduled_updates_.begin();
       i != scheduled_updates_.end(); ++i) {
    const TabContents* contents = i->first;
    unsigned flags = i->second;

    if (contents == GetSelectedTabContents()) {

      if (flags & TabContents::INVALIDATE_PAGE_ACTIONS) {
        LocationBar* location_bar = window()->GetLocationBar();
        if (location_bar)
          location_bar->UpdatePageActions();
      }
      if (flags & TabContents::INVALIDATE_LOAD && GetStatusBubble()) {
        GetStatusBubble()->SetStatus(
            GetSelectedTabContentsWrapper()->GetStatusText());
      }

      if (flags & (TabContents::INVALIDATE_TAB |
                   TabContents::INVALIDATE_TITLE)) {
#if !defined(OS_MACOSX)
        command_updater_.UpdateCommandEnabled(IDC_CREATE_SHORTCUTS,
            web_app::IsValidUrl(contents->GetURL()));
#endif
        window_->UpdateTitleBar();
      }
    }

    if (flags & (TabContents::INVALIDATE_TAB | TabContents::INVALIDATE_TITLE)) {
      tab_handler_->GetTabStripModel()->UpdateTabContentsStateAt(
          tab_handler_->GetTabStripModel()->GetWrapperIndex(contents),
          TabStripModelObserver::ALL);
    }

  }

  scheduled_updates_.clear();
}
