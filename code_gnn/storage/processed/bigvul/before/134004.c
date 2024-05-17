void ExtensionAppItem::Activate(int event_flags) {
  const Extension* extension = GetExtension();
  if (!extension)
    return;

  if (!extensions::util::IsAppLaunchable(extension_id_, profile_))
    return;

  if (RunExtensionEnableFlow())
    return;

  content::RecordAction(base::UserMetricsAction("AppList_ClickOnApp"));
  extensions::RecordAppListMainLaunch(extension);
  GetController()->ActivateApp(profile_,
                               extension,
                               AppListControllerDelegate::LAUNCH_FROM_APP_LIST,
                               event_flags);
}
