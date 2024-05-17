void SessionService::BuildCommandsForTab(
    const SessionID& window_id,
    TabContentsWrapper* tab,
    int index_in_window,
    bool is_pinned,
    std::vector<SessionCommand*>* commands,
    IdToRange* tab_to_available_range) {
  DCHECK(tab && commands && window_id.id());
  const SessionID& session_id(tab->restore_tab_helper()->session_id());
  commands->push_back(CreateSetTabWindowCommand(window_id, session_id));
  const int current_index =
      tab->web_contents()->GetController().GetCurrentEntryIndex();
  const int min_index = std::max(0,
                                 current_index - max_persist_navigation_count);
  const int max_index =
      std::min(current_index + max_persist_navigation_count,
               tab->web_contents()->GetController().GetEntryCount());
  const int pending_index =
      tab->web_contents()->GetController().GetPendingEntryIndex();
  if (tab_to_available_range) {
    (*tab_to_available_range)[session_id.id()] =
        std::pair<int, int>(min_index, max_index);
  }
  if (is_pinned) {
    commands->push_back(CreatePinnedStateCommand(session_id, true));
  }
  TabContentsWrapper* wrapper =
      TabContentsWrapper::GetCurrentWrapperForContents(tab->web_contents());
  if (wrapper->extension_tab_helper()->extension_app()) {
    commands->push_back(
        CreateSetTabExtensionAppIDCommand(
            kCommandSetExtensionAppID, session_id.id(),
            wrapper->extension_tab_helper()->extension_app()->id()));
  }
  for (int i = min_index; i < max_index; ++i) {
    const NavigationEntry* entry = (i == pending_index) ?
        tab->web_contents()->GetController().GetPendingEntry() :
        tab->web_contents()->GetController().GetEntryAtIndex(i);
    DCHECK(entry);
    if (ShouldTrackEntry(entry->GetVirtualURL())) {
      commands->push_back(
          CreateUpdateTabNavigationCommand(
              kCommandUpdateTabNavigation, session_id.id(), i, *entry));
    }
  }
  commands->push_back(
      CreateSetSelectedNavigationIndexCommand(session_id, current_index));

  if (index_in_window != -1) {
    commands->push_back(
        CreateSetTabIndexInWindowCommand(session_id, index_in_window));
  }
}
