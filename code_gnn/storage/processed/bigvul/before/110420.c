bool WebUILoginView::TakeFocus(bool reverse) {
  ash::SystemTray* tray = ash::Shell::GetInstance()->system_tray();
  if (tray && tray->GetWidget()->IsVisible()) {
    tray->SetNextFocusableView(this);
    ash::Shell::GetInstance()->RotateFocus(reverse ? ash::Shell::BACKWARD :
                                                     ash::Shell::FORWARD);
  }

   return true;
 }
