void ExtensionInstallUI::OpenAppInstalledNTP(Browser* browser,
                                             const std::string& app_id) {
  browser::NavigateParams params =
      browser->GetSingletonTabNavigateParams(GURL(chrome::kChromeUINewTabURL));
  browser::Navigate(&params);

  content::NotificationService::current()->Notify(
      chrome::NOTIFICATION_APP_INSTALLED_TO_NTP,
      content::Source<WebContents>(params.target_contents->web_contents()),
      content::Details<const std::string>(&app_id));
}