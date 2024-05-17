 void PaletteTray::UpdateIconVisibility() {
  if (!IsPaletteEnabled())
    return;

  SessionStateDelegate* session_state_delegate =
      WmShell::Get()->GetSessionStateDelegate();

  SetVisible(!session_state_delegate->IsScreenLocked() &&
             session_state_delegate->GetSessionState() ==
                 SessionStateDelegate::SESSION_STATE_ACTIVE &&
             WmShell::Get()->system_tray_delegate()->GetUserLoginStatus() !=
                 LoginStatus::KIOSK_APP);
}
