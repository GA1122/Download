void AppListControllerDelegateWin::LaunchApp(
    Profile* profile, const extensions::Extension* extension, int event_flags) {
  AppListServiceImpl::RecordAppListAppLaunch();
  chrome::OpenApplication(chrome::AppLaunchParams(
      profile, extension, NEW_FOREGROUND_TAB));
}
