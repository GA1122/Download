void RecordAppLaunches(
    Profile* profile,
    const std::vector<GURL>& cmd_line_urls,
    const std::vector<BrowserInit::LaunchWithProfile::Tab>& autolaunch_tabs) {
  ExtensionService* extension_service = profile->GetExtensionService();
  DCHECK(extension_service);
  for (size_t i = 0; i < cmd_line_urls.size(); ++i) {
    if (extension_service->IsInstalledApp(cmd_line_urls.at(i))) {
      UMA_HISTOGRAM_ENUMERATION(extension_misc::kAppLaunchHistogram,
                                extension_misc::APP_LAUNCH_CMD_LINE_URL,
                                extension_misc::APP_LAUNCH_BUCKET_BOUNDARY);
    }
  }
  for (size_t i = 0; i < autolaunch_tabs.size(); ++i) {
    if (extension_service->IsInstalledApp(autolaunch_tabs.at(i).url)) {
      UMA_HISTOGRAM_ENUMERATION(extension_misc::kAppLaunchHistogram,
                                extension_misc::APP_LAUNCH_AUTOLAUNCH,
                                extension_misc::APP_LAUNCH_BUCKET_BOUNDARY);
    }
  }
}
