void Browser::ProcessPendingUIUpdates() {
#ifndef NDEBUG
  for (UpdateMap::const_iterator i = scheduled_updates_.begin();
       i != scheduled_updates_.end(); ++i) {
    bool found = false;
    for (int tab = 0; tab < tab_count(); tab++) {
      if (chrome::GetWebContentsAt(this, tab) == i->first) {
        found = true;
        break;
      }
    }
    DCHECK(found);
  }
#endif

  chrome_updater_factory_.InvalidateWeakPtrs();

  for (UpdateMap::const_iterator i = scheduled_updates_.begin();
       i != scheduled_updates_.end(); ++i) {
    const WebContents* contents = i->first;
    unsigned flags = i->second;

    if (contents == chrome::GetActiveWebContents(this)) {

      if (flags & content::INVALIDATE_TYPE_PAGE_ACTIONS) {
        LocationBar* location_bar = window()->GetLocationBar();
        if (location_bar)
          location_bar->UpdatePageActions();
      }
      if (flags & content::INVALIDATE_TYPE_LOAD && GetStatusBubble()) {
        GetStatusBubble()->SetStatus(
            CoreTabHelper::FromWebContents(chrome::GetActiveWebContents(this))->
                GetStatusText());
      }

      if (flags & (content::INVALIDATE_TYPE_TAB |
                   content::INVALIDATE_TYPE_TITLE)) {
        window_->UpdateTitleBar();
      }
    }

    if (flags &
        (content::INVALIDATE_TYPE_TAB | content::INVALIDATE_TYPE_TITLE)) {
      tab_strip_model_->UpdateTabContentsStateAt(
          tab_strip_model_->GetIndexOfWebContents(contents),
          TabStripModelObserver::ALL);
    }

  }

  scheduled_updates_.clear();
}
