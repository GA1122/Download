void SessionService::TabRestored(TabContentsWrapper* tab, bool pinned) {
  if (!ShouldTrackChangesToWindow(tab->restore_tab_helper()->window_id()))
    return;

  BuildCommandsForTab(tab->restore_tab_helper()->window_id(), tab, -1,
                      pinned, &pending_commands(), NULL);
  StartSaveTimer();
}
