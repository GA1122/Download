bool AppListControllerDelegate::CanDoShowAppInfoFlow() {
#if defined(OS_MACOSX)
  if (!app_list::switches::IsMacViewsAppListEnabled())
    return false;
#endif
  return CanShowAppInfoDialog();
}
