bool BrowserInit::LaunchWithProfile::OpenApplicationTab(Profile* profile) {
  std::string app_id;
  if (!IsAppLaunch(NULL, &app_id) || app_id.empty())
    return false;

  extension_misc::LaunchContainer launch_container;
  const Extension* extension;
  if (!GetAppLaunchContainer(profile, app_id, &extension, &launch_container))
    return false;

  if (launch_container != extension_misc::LAUNCH_TAB)
    return false;

  RecordCmdLineAppHistogram();

  WebContents* app_tab = Browser::OpenApplicationTab(profile, extension, GURL(),
                                                     NEW_FOREGROUND_TAB);
  return (app_tab != NULL);
}
