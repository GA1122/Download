void WindowTreeHostManager::Start() {
  display::Screen::GetScreen()->AddObserver(this);
  Shell::GetInstance()->display_manager()->set_delegate(this);
}
