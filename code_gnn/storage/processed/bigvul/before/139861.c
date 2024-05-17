void Shell::ToggleAppList(aura::Window* window) {
  if (app_list_controller_ && app_list_controller_->IsVisible()) {
    DismissAppList();
    return;
  }

  ShowAppList(window);
}
