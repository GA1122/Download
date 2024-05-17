void UserSelectionScreen::ShowUserPodCustomIcon(
    const AccountId& account_id,
    const proximity_auth::ScreenlockBridge::UserPodCustomIconOptions&
        icon_options) {
  view_->ShowUserPodCustomIcon(account_id, icon_options);
}
