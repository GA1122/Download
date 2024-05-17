void UserSelectionScreen::InitEasyUnlock() {
  proximity_auth::ScreenlockBridge::Get()->SetLockHandler(this);
}
