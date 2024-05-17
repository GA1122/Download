void OneClickSigninSyncStarter::SigninFailed(
    const GoogleServiceAuthError& error) {
  FinishProfileSyncServiceSetup();
  if (confirmation_required_ == CONFIRM_AFTER_SIGNIN) {
    switch (error.state()) {
      case GoogleServiceAuthError::SERVICE_UNAVAILABLE:
        DisplayFinalConfirmationBubble(l10n_util::GetStringUTF16(
            IDS_SYNC_UNRECOVERABLE_ERROR));
        break;
      case GoogleServiceAuthError::REQUEST_CANCELED:
        break;
      default:
        DisplayFinalConfirmationBubble(l10n_util::GetStringUTF16(
            IDS_SYNC_ERROR_SIGNING_IN));
        break;
    }
  }
  delete this;
}
