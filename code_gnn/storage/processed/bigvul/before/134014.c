void ExtensionAppItem::Launch(int event_flags) {
  const Extension* extension = GetExtension();
  if (!extension)
    return;

  if (!extensions::util::IsAppLaunchable(extension_id_, profile_))
    return;

  if (RunExtensionEnableFlow())
    return;

  GetController()->LaunchApp(profile_,
                             extension,
                             AppListControllerDelegate::LAUNCH_FROM_APP_LIST,
                             event_flags);
}
