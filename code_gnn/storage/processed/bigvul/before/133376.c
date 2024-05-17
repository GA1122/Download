void WindowTreeHostManager::Shutdown() {
  FOR_EACH_OBSERVER(Observer, observers_, OnWindowTreeHostManagerShutdown());

  Shell::GetInstance()->display_manager()->set_delegate(nullptr);

  cursor_window_controller_.reset();
  mirror_window_controller_.reset();

  display::Screen::GetScreen()->RemoveObserver(this);

  int64_t primary_id = display::Screen::GetScreen()->GetPrimaryDisplay().id();

  aura::Window::Windows root_windows =
      WindowTreeHostManager::GetAllRootWindows();
  std::vector<RootWindowController*> to_delete;
  RootWindowController* primary_rwc = nullptr;
  for (aura::Window::Windows::iterator iter = root_windows.begin();
       iter != root_windows.end(); ++iter) {
    RootWindowController* rwc = GetRootWindowController(*iter);
    if (GetRootWindowSettings(*iter)->display_id == primary_id)
      primary_rwc = rwc;
    else
      to_delete.push_back(rwc);
  }
  CHECK(primary_rwc);

  base::STLDeleteElements(&to_delete);
  delete primary_rwc;
}
