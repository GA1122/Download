void ProfileChooserView::OnRefreshTokenAvailable(
    const std::string& account_id) {
  if (view_mode_ == profiles::BUBBLE_VIEW_MODE_ACCOUNT_MANAGEMENT ||
      view_mode_ == profiles::BUBBLE_VIEW_MODE_GAIA_ADD_ACCOUNT ||
      view_mode_ == profiles::BUBBLE_VIEW_MODE_GAIA_REAUTH) {
    ShowViewFromMode(AccountConsistencyModeManager::IsMirrorEnabledForProfile(
                         browser_->profile())
                         ? profiles::BUBBLE_VIEW_MODE_ACCOUNT_MANAGEMENT
                         : profiles::BUBBLE_VIEW_MODE_PROFILE_CHOOSER);
  }
}
