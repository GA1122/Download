void BrowserView::ShowOneClickSigninConfirmation(
    const base::string16& email,
    const StartSyncCallback& start_sync_callback) {
  std::unique_ptr<OneClickSigninLinksDelegate> delegate(
      new OneClickSigninLinksDelegateImpl(browser()));
  OneClickSigninDialogView::ShowDialog(email, std::move(delegate),
                                       GetNativeWindow(), start_sync_callback);
}
