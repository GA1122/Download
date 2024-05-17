WebUILoginView::~WebUILoginView() {
  ash::SystemTray* tray = ash::Shell::GetInstance()->system_tray();
  if (tray)
    tray->SetNextFocusableView(NULL);
}
