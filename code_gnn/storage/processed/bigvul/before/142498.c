void ShelfLayoutManager::UpdateVisibilityState() {
  aura::Window* shelf_window = shelf_widget_->GetNativeWindow();
  if (in_shutdown_ || !shelf_window)
    return;

  wm::WorkspaceWindowState window_state(
      RootWindowController::ForWindow(shelf_window)->GetWorkspaceWindowState());

  if (shelf_->ShouldHideOnSecondaryDisplay(state_.session_state)) {
    SetState(SHELF_HIDDEN);
  } else if (!state_.IsActiveSessionState()) {
    SetState(SHELF_VISIBLE);
  } else if (Shell::Get()->screen_pinning_controller()->IsPinned()) {
    SetState(SHELF_HIDDEN);
  } else {
    switch (window_state) {
      case wm::WORKSPACE_WINDOW_STATE_FULL_SCREEN:
        if (IsShelfAutoHideForFullscreenMaximized()) {
          SetState(SHELF_AUTO_HIDE);
        } else if (IsShelfHiddenForFullscreen()) {
          SetState(SHELF_HIDDEN);
        } else {
          SetState(SHELF_AUTO_HIDE);
        }
        break;
      case wm::WORKSPACE_WINDOW_STATE_MAXIMIZED:
        SetState(IsShelfAutoHideForFullscreenMaximized()
                     ? SHELF_AUTO_HIDE
                     : CalculateShelfVisibility());
        break;
      case wm::WORKSPACE_WINDOW_STATE_DEFAULT:
        SetState(CalculateShelfVisibility());
        break;
    }
  }

  UpdateWorkspaceMask(window_state);
}
