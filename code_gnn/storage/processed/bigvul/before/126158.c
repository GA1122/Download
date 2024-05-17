void Browser::ActiveTabChanged(TabContents* old_contents,
                               TabContents* new_contents,
                               int index,
                               bool user_gesture) {
  bool did_reload = false;
  if (user_gesture && ShouldReloadCrashedTab(new_contents->web_contents())) {
    LOG(WARNING) << "Reloading killed tab at " << index;
    static int reload_count = 0;
    UMA_HISTOGRAM_CUSTOM_COUNTS(
        "Tabs.SadTab.ReloadCount", ++reload_count, 1, 1000, 50);
    chrome::Reload(this, CURRENT_TAB);
    did_reload = true;
  }

  if (!did_reload && tab_strip_model_->IsTabDiscarded(index)) {
    LOG(WARNING) << "Reloading discarded tab at " << index;
    static int reload_count = 0;
    UMA_HISTOGRAM_CUSTOM_COUNTS(
        "Tabs.Discard.ReloadCount", ++reload_count, 1, 1000, 50);
    chrome::Reload(this, CURRENT_TAB);
  }

  if (chrome_updater_factory_.HasWeakPtrs() && old_contents)
    ProcessPendingUIUpdates();

  UpdateToolbar(true);

  UpdateSearchState(new_contents);

  command_controller_->LoadingStateChanged(
      new_contents->web_contents()->IsLoading(), true);

  command_controller_->TabStateChanged();

  StatusBubble* status_bubble = GetStatusBubble();
  if (status_bubble) {
    status_bubble->Hide();

    status_bubble->SetStatus(
        CoreTabHelper::FromWebContents(chrome::GetActiveWebContents(this))->
            GetStatusText());
  }

  if (HasFindBarController()) {
    find_bar_controller_->ChangeWebContents(new_contents->web_contents());
    find_bar_controller_->find_bar()->MoveWindowIfNecessary(gfx::Rect(), true);
  }

  SessionService* session_service =
      SessionServiceFactory::GetForProfileIfExisting(profile_);
  if (session_service && !tab_strip_model_->closing_all()) {
    session_service->SetSelectedTabInWindow(session_id(), active_index());
  }

  UpdateBookmarkBarState(BOOKMARK_BAR_STATE_CHANGE_TAB_SWITCH);
}
