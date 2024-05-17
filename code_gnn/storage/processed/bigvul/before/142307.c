void ChromePasswordManagerClient::NavigateToManagePasswordsPage(
    password_manager::ManagePasswordsReferrer referrer) {
#if defined(OS_ANDROID)
  chrome::android::PreferencesLauncher::ShowPasswordSettings(web_contents(),
                                                             referrer);
#else
  ::NavigateToManagePasswordsPage(
      chrome::FindBrowserWithWebContents(web_contents()), referrer);
#endif
}
