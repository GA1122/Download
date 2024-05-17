void ProfileChooserView::ButtonPressed(views::Button* sender,
                                       const ui::Event& event) {
  if (sender == passwords_button_) {
    base::RecordAction(
        base::UserMetricsAction("ProfileChooser_PasswordsClicked"));
    chrome::ShowSettingsSubPage(browser_, chrome::kPasswordManagerSubPage);
  } else if (sender == credit_cards_button_) {
    base::RecordAction(
        base::UserMetricsAction("ProfileChooser_PaymentsClicked"));
    chrome::ShowSettingsSubPage(browser_, chrome::kPaymentsSubPage);
  } else if (sender == addresses_button_) {
    base::RecordAction(
        base::UserMetricsAction("ProfileChooser_AddressesClicked"));
    chrome::ShowSettingsSubPage(browser_, chrome::kAutofillSubPage);
  } else if (sender == guest_profile_button_) {
    PrefService* service = g_browser_process->local_state();
    DCHECK(service);
    DCHECK(service->GetBoolean(prefs::kBrowserGuestModeEnabled));
    profiles::SwitchToGuestProfile(ProfileManager::CreateCallback());
    base::RecordAction(base::UserMetricsAction("ProfileChooser_GuestClicked"));
  } else if (sender == users_button_) {
    if (browser_->profile()->IsGuestSession()) {
      profiles::CloseGuestProfileWindows();
    } else {
      base::RecordAction(
          base::UserMetricsAction("ProfileChooser_ManageClicked"));
      UserManager::Show(base::FilePath(),
                        profiles::USER_MANAGER_SELECT_PROFILE_NO_ACTION);
    }
    PostActionPerformed(ProfileMetrics::PROFILE_DESKTOP_MENU_OPEN_USER_MANAGER);
  } else if (sender == go_incognito_button_) {
    DCHECK(ShouldShowGoIncognito());
    chrome::NewIncognitoWindow(browser_->profile());
    PostActionPerformed(ProfileMetrics::PROFILE_DESKTOP_MENU_GO_INCOGNITO);
  } else if (sender == lock_button_) {
    profiles::LockProfile(browser_->profile());
    PostActionPerformed(ProfileMetrics::PROFILE_DESKTOP_MENU_LOCK);
  } else if (sender == close_all_windows_button_) {
    profiles::CloseProfileWindows(browser_->profile());
    base::RecordAction(
        base::UserMetricsAction("ProfileChooser_CloseAllClicked"));
  } else if (sender == sync_error_button_) {
    sync_ui_util::AvatarSyncErrorType error =
        static_cast<sync_ui_util::AvatarSyncErrorType>(sender->id());
    switch (error) {
      case sync_ui_util::MANAGED_USER_UNRECOVERABLE_ERROR:
        chrome::ShowSettingsSubPage(browser_, chrome::kSignOutSubPage);
        break;
      case sync_ui_util::UNRECOVERABLE_ERROR:
        if (ProfileSyncServiceFactory::GetForProfile(browser_->profile())) {
          browser_sync::ProfileSyncService::SyncEvent(
              browser_sync::ProfileSyncService::STOP_FROM_OPTIONS);
        }
        SigninManagerFactory::GetForProfile(browser_->profile())
            ->SignOut(signin_metrics::USER_CLICKED_SIGNOUT_SETTINGS,
                      signin_metrics::SignoutDelete::IGNORE_METRIC);
        ShowViewFromMode(profiles::BUBBLE_VIEW_MODE_GAIA_SIGNIN);
        break;
      case sync_ui_util::SUPERVISED_USER_AUTH_ERROR:
        NOTREACHED();
        break;
      case sync_ui_util::AUTH_ERROR:
        ShowViewFromMode(profiles::BUBBLE_VIEW_MODE_GAIA_REAUTH);
        break;
      case sync_ui_util::UPGRADE_CLIENT_ERROR:
        chrome::OpenUpdateChromeDialog(browser_);
        break;
      case sync_ui_util::PASSPHRASE_ERROR:
      case sync_ui_util::SETTINGS_UNCONFIRMED_ERROR:
        chrome::ShowSettingsSubPage(browser_, chrome::kSyncSetupSubPage);
        break;
      case sync_ui_util::NO_SYNC_ERROR:
        NOTREACHED();
        break;
    }
    base::RecordAction(
        base::UserMetricsAction("ProfileChooser_SignInAgainClicked"));
  } else if (sender == remove_account_button_) {
    RemoveAccount();
  } else if (sender == account_removal_cancel_button_) {
    account_id_to_remove_.clear();
    ShowViewFromMode(profiles::BUBBLE_VIEW_MODE_ACCOUNT_MANAGEMENT);
  } else if (sender == gaia_signin_cancel_button_) {
    Profile* profile = browser_->profile();
    bool account_management_available =
        SigninManagerFactory::GetForProfile(profile)->IsAuthenticated() &&
        AccountConsistencyModeManager::IsMirrorEnabledForProfile(profile);
    ShowViewFromMode(account_management_available ?
        profiles::BUBBLE_VIEW_MODE_ACCOUNT_MANAGEMENT :
        profiles::BUBBLE_VIEW_MODE_PROFILE_CHOOSER);
  } else if (sender == current_profile_card_) {
    if (dice_enabled_ &&
        SigninManagerFactory::GetForProfile(browser_->profile())
            ->IsAuthenticated()) {
      chrome::ShowSettingsSubPage(browser_, chrome::kPeopleSubPage);
    } else {
      avatar_menu_->EditProfile(avatar_menu_->GetActiveProfileIndex());
      PostActionPerformed(ProfileMetrics::PROFILE_DESKTOP_MENU_EDIT_IMAGE);
      PostActionPerformed(ProfileMetrics::PROFILE_DESKTOP_MENU_EDIT_NAME);
    }
  } else if (sender == manage_accounts_button_) {
    ShowViewFromMode(view_mode_ == profiles::BUBBLE_VIEW_MODE_ACCOUNT_MANAGEMENT
                         ? profiles::BUBBLE_VIEW_MODE_PROFILE_CHOOSER
                         : profiles::BUBBLE_VIEW_MODE_ACCOUNT_MANAGEMENT);
  } else if (sender == signin_current_profile_button_) {
    ShowViewFromMode(profiles::BUBBLE_VIEW_MODE_GAIA_SIGNIN);
  } else if (sender == signin_with_gaia_account_button_) {
    DCHECK(dice_signin_button_view_->account());
    Hide();
    signin_ui_util::EnableSyncFromPromo(
        browser_, dice_signin_button_view_->account().value(), access_point_,
        true  );
  } else if (sender == sync_to_another_account_button_) {
    std::vector<AccountInfo> accounts(dice_sync_promo_accounts_.begin() + 1,
                                      dice_sync_promo_accounts_.end());
    dice_accounts_menu_ = std::make_unique<DiceAccountsMenu>(
        accounts, GetImagesForAccounts(accounts, browser_->profile()),
        base::BindOnce(&ProfileChooserView::EnableSync,
                       base::Unretained(this)));
    dice_accounts_menu_->SetSignOutButtonCallback(base::BindOnce(
        &ProfileChooserView::SignOutAllWebAccounts, base::Unretained(this)));
    dice_accounts_menu_->Show(sender, sync_to_another_account_button_);
  } else if (sender == signout_button_) {
    SignOutAllWebAccounts();
    base::RecordAction(base::UserMetricsAction("Signin_Signout_FromUserMenu"));
  } else {
    ButtonIndexes::const_iterator profile_match =
        open_other_profile_indexes_map_.find(sender);
    if (profile_match != open_other_profile_indexes_map_.end()) {
      avatar_menu_->SwitchToProfile(
          profile_match->second, ui::DispositionFromEventFlags(event.flags()) ==
                                     WindowOpenDisposition::NEW_WINDOW,
          ProfileMetrics::SWITCH_PROFILE_ICON);
      base::RecordAction(
          base::UserMetricsAction("ProfileChooser_ProfileClicked"));
      Hide();
    } else {
      AccountButtonIndexes::const_iterator account_match =
          delete_account_button_map_.find(sender);
      if (account_match != delete_account_button_map_.end()) {
        account_id_to_remove_ = account_match->second;
        ShowViewFromMode(profiles::BUBBLE_VIEW_MODE_ACCOUNT_REMOVAL);
      } else {
        account_match = reauth_account_button_map_.find(sender);
        DCHECK(account_match != reauth_account_button_map_.end());
        ShowViewFromMode(profiles::BUBBLE_VIEW_MODE_GAIA_REAUTH);
      }
    }
  }
}
