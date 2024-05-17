void Browser::ActiveTabChanged(TabContentsWrapper* old_contents,
                               TabContentsWrapper* new_contents,
                               int index,
                               bool user_gesture) {
  if (user_gesture && new_contents->tab_contents()->crashed_status() ==
        base::TERMINATION_STATUS_PROCESS_WAS_KILLED) {
    const CommandLine& parsed_command_line = *CommandLine::ForCurrentProcess();
    if (parsed_command_line.HasSwitch(switches::kReloadKilledTabs)) {
      Reload(CURRENT_TAB);
      return;
    }
  }

  if (!chrome_updater_factory_.empty() && old_contents)
    ProcessPendingUIUpdates();

  UpdateToolbar(true);

  UpdateReloadStopState(new_contents->tab_contents()->IsLoading(), true);

  UpdateCommandsForTabState();

  StatusBubble* status_bubble = GetStatusBubble();
  if (status_bubble) {
    status_bubble->Hide();

    status_bubble->SetStatus(GetSelectedTabContentsWrapper()->GetStatusText());
  }

  if (HasFindBarController()) {
    find_bar_controller_->ChangeTabContents(new_contents);
    find_bar_controller_->find_bar()->MoveWindowIfNecessary(gfx::Rect(), true);
  }

  SessionService* session_service =
      SessionServiceFactory::GetForProfileIfExisting(profile_);
  if (session_service && !tab_handler_->GetTabStripModel()->closing_all()) {
    session_service->SetSelectedTabInWindow(
        session_id(), tab_handler_->GetTabStripModel()->active_index());
  }

  UpdateBookmarkBarState(BOOKMARK_BAR_STATE_CHANGE_TAB_SWITCH);
}
