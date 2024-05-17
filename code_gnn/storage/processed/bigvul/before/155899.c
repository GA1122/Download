void ProfileChooserView::Init() {
  set_close_on_deactivate(close_on_deactivate_for_testing_);

  avatar_menu_.reset(new AvatarMenu(
      &g_browser_process->profile_manager()->GetProfileAttributesStorage(),
      this, browser_));
  avatar_menu_->RebuildMenu();

  Profile* profile = browser_->profile();
  ProfileOAuth2TokenService* oauth2_token_service =
      ProfileOAuth2TokenServiceFactory::GetForProfile(profile);
  if (oauth2_token_service)
    oauth2_token_service->AddObserver(this);

  if (IsProfileChooser(view_mode_) && HasAuthError(profile) &&
      AccountConsistencyModeManager::IsMirrorEnabledForProfile(profile) &&
      avatar_menu_->GetItemAt(avatar_menu_->GetActiveProfileIndex())
          .signed_in) {
    view_mode_ = profiles::BUBBLE_VIEW_MODE_ACCOUNT_MANAGEMENT;
  }

  AddAccelerator(ui::Accelerator(ui::VKEY_DOWN, ui::EF_NONE));
  AddAccelerator(ui::Accelerator(ui::VKEY_UP, ui::EF_NONE));

  ShowViewFromMode(view_mode_);
}
