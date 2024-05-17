void OneClickSigninSyncStarter::SigninSuccess() {
  switch (start_mode_) {
    case SYNC_WITH_DEFAULT_SETTINGS: {
      ProfileSyncService* profile_sync_service = GetProfileSyncService();
      if (profile_sync_service)
        profile_sync_service->SetSyncSetupCompleted();
      FinishProfileSyncServiceSetup();
      if (confirmation_required_ == CONFIRM_AFTER_SIGNIN) {
        string16 message;
        if (!profile_sync_service) {
          message = l10n_util::GetStringUTF16(
              IDS_ONE_CLICK_SIGNIN_BUBBLE_SYNC_DISABLED_MESSAGE);
        }
        DisplayFinalConfirmationBubble(message);
      }
      break;
    }
    case CONFIGURE_SYNC_FIRST:
      ConfigureSync();
      break;
    default:
      NOTREACHED();
  }
  delete this;
}
