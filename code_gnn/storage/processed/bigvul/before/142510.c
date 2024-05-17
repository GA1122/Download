  wm::WorkspaceWindowState GetWorkspaceWindowState() const {
    const auto* shelf_window = GetShelfWidget()->GetNativeWindow();
    return RootWindowController::ForWindow(shelf_window)
        ->GetWorkspaceWindowState();
  }
