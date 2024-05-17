void ShelfLayoutManager::UpdateVisibilityState() {
  ShellDelegate* delegate = Shell::GetInstance()->delegate();
  if (delegate && delegate->IsScreenLocked()) {
    SetState(VISIBLE);
  } else {
    WorkspaceManager::WindowState window_state(
        workspace_manager_->GetWindowState());
    switch (window_state) {
      case WorkspaceManager::WINDOW_STATE_FULL_SCREEN:
        SetState(HIDDEN);
        break;

      case WorkspaceManager::WINDOW_STATE_MAXIMIZED:
        SetState(auto_hide_behavior_ != SHELF_AUTO_HIDE_BEHAVIOR_NEVER ?
                 AUTO_HIDE : VISIBLE);
        break;

      case WorkspaceManager::WINDOW_STATE_WINDOW_OVERLAPS_SHELF:
      case WorkspaceManager::WINDOW_STATE_DEFAULT:
        SetState(auto_hide_behavior_ == SHELF_AUTO_HIDE_BEHAVIOR_ALWAYS ?
                 AUTO_HIDE : VISIBLE);
        SetWindowOverlapsShelf(window_state ==
            WorkspaceManager::WINDOW_STATE_WINDOW_OVERLAPS_SHELF);
    }
  }
}
