void OneClickSigninSyncStarter::SigninDialogDelegate::OnCancelSignin() {
  sync_starter_->CancelSigninAndDelete();
}
