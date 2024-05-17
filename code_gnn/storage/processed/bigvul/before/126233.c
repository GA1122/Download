void Browser::ScheduleUIUpdate(const WebContents* source,
                               unsigned changed_flags) {
  if (!source)
    return;

  if (changed_flags & content::INVALIDATE_TYPE_URL &&
      source == chrome::GetActiveWebContents(this)) {
    UpdateToolbar(false);
    changed_flags &= ~content::INVALIDATE_TYPE_URL;
  }
  if (changed_flags & content::INVALIDATE_TYPE_LOAD) {
    tab_strip_model_->UpdateTabContentsStateAt(
        chrome::GetIndexOfTab(this, source),
        TabStripModelObserver::LOADING_ONLY);
  }

  if (changed_flags & content::INVALIDATE_TYPE_TITLE && !source->IsLoading()) {
    tab_strip_model_->UpdateTabContentsStateAt(
        chrome::GetIndexOfTab(this, source),
        TabStripModelObserver::TITLE_NOT_LOADING);
  }

  if (changed_flags == 0)
    return;

  scheduled_updates_[source] |= changed_flags;

  if (!chrome_updater_factory_.HasWeakPtrs()) {
    MessageLoop::current()->PostDelayedTask(
        FROM_HERE,
        base::Bind(&Browser::ProcessPendingUIUpdates,
                   chrome_updater_factory_.GetWeakPtr()),
        base::TimeDelta::FromMilliseconds(kUIUpdateCoalescingTimeMS));
  }
}
