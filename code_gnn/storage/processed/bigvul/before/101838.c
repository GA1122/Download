void Browser::TabDetachedAtImpl(TabContentsWrapper* contents, int index,
                                DetachType type) {
  if (type == DETACH_TYPE_DETACH) {
    if (contents == GetSelectedTabContentsWrapper()) {
      LocationBar* location_bar = window()->GetLocationBar();
      if (location_bar)
        location_bar->SaveStateToContents(contents->tab_contents());
    }

    if (!tab_handler_->GetTabStripModel()->closing_all())
      SyncHistoryWithTabs(0);
  }

  SetAsDelegate(contents, NULL);
  RemoveScheduledUpdatesFor(contents->tab_contents());

  if (find_bar_controller_.get() &&
      index == tab_handler_->GetTabStripModel()->active_index()) {
    find_bar_controller_->ChangeTabContents(NULL);
  }

  if (is_attempting_to_close_browser_) {
    ClearUnloadState(contents->tab_contents(), false);
  }

  registrar_.Remove(this, content::NOTIFICATION_INTERSTITIAL_ATTACHED,
                    Source<TabContents>(contents->tab_contents()));
  registrar_.Remove(this, content::NOTIFICATION_TAB_CONTENTS_DISCONNECTED,
                    Source<TabContents>(contents->tab_contents()));
}
