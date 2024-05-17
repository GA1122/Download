void UserSelectionScreen::Unlock(const AccountId& account_id) {
  DCHECK_EQ(GetScreenType(), LOCK_SCREEN);
  ScreenLocker::Hide();
}
