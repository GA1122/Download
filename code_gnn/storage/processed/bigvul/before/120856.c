void StartExplicitSync(const StartSyncArgs& args,
                       content::WebContents* contents,
                       OneClickSigninSyncStarter::StartSyncMode start_mode,
                       int button) {
  if (button == IDS_ONE_CLICK_SIGNIN_CONFIRM_EMAIL_DIALOG_OK_BUTTON) {
    contents->GetController().LoadURL(
        GURL(chrome::kChromeUINewTabURL), content::Referrer(),
        content::PAGE_TRANSITION_AUTO_TOPLEVEL, std::string());
    chrome::ShowSettingsSubPage(args.browser,
                                std::string(chrome::kSearchUsersSubPage));
  } else {
    StartSync(args, start_mode);
    RedirectToNtpOrAppsPageIfNecessary(contents, args.source);
  }
}
