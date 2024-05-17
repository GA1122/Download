ScreenLayoutObserver::ScreenLayoutObserver() {
  ShellPort::Get()->AddDisplayObserver(this);
  UpdateDisplayInfo(NULL);
}
