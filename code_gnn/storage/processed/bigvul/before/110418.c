void WebUILoginView::SetStatusAreaVisible(bool visible) {
  ash::SystemTray* tray = ash::Shell::GetInstance()->system_tray();
  if (tray) {
    if (visible)
      tray->GetWidget()->Show();
    else
      tray->GetWidget()->Hide();
  }
}
