void Browser::ScheduleUIUpdate(const TabContents* source,
                               unsigned changed_flags) {
  if (!source)
    return;

  if (changed_flags & TabContents::INVALIDATE_URL &&
      source == GetSelectedTabContents()) {
    UpdateToolbar(false);
    changed_flags &= ~TabContents::INVALIDATE_URL;
  }
  if (changed_flags & TabContents::INVALIDATE_LOAD) {
    tab_handler_->GetTabStripModel()->UpdateTabContentsStateAt(
        tab_handler_->GetTabStripModel()->GetIndexOfController(
            &source->controller()),
        TabStripModelObserver::LOADING_ONLY);
  }

  if (changed_flags & TabContents::INVALIDATE_TITLE && !source->is_loading()) {
    tab_handler_->GetTabStripModel()->UpdateTabContentsStateAt(
        tab_handler_->GetTabStripModel()->GetIndexOfController(
            &source->controller()),
        TabStripModelObserver::TITLE_NOT_LOADING);
  }

  if (changed_flags & TabContents::INVALIDATE_BOOKMARK_BAR) {
    window()->ShelfVisibilityChanged();
    changed_flags &= ~TabContents::INVALIDATE_BOOKMARK_BAR;
  }

  if (changed_flags == 0)
    return;

  scheduled_updates_[source] |= changed_flags;

  if (chrome_updater_factory_.empty()) {
    MessageLoop::current()->PostDelayedTask(
        FROM_HERE,
        chrome_updater_factory_.NewRunnableMethod(
            &Browser::ProcessPendingUIUpdates),
            kUIUpdateCoalescingTimeMS);
  }
}
