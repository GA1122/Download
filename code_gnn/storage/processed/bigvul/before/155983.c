void CloseModalSigninIfNeeded(InlineLoginHandlerImpl* handler) {
  if (handler) {
    Browser* browser = handler->GetDesktopBrowser();
    if (browser)
      browser->signin_view_controller()->CloseModalSignin();
  }
}
