void OneClickSigninSyncStarter::DisplayFinalConfirmationBubble(
    const string16& custom_message) {
  EnsureBrowser();
  browser_->window()->ShowOneClickSigninBubble(
      BrowserWindow::ONE_CLICK_SIGNIN_BUBBLE_TYPE_BUBBLE,
      string16(),   
      custom_message,
      BrowserWindow::StartSyncCallback());
}
