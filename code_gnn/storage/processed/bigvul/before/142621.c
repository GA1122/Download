void TabHelper::SetExtensionApp(const Extension* extension) {
  DCHECK(!extension || AppLaunchInfo::GetFullLaunchURL(extension).is_valid());
  if (extension_app_ == extension)
    return;

  extension_app_ = extension;

  if (extension_app_) {
    registry_observer_.Add(
        ExtensionRegistry::Get(web_contents()->GetBrowserContext()));
  } else {
    registry_observer_.RemoveAll();
  }

  UpdateExtensionAppIcon(extension_app_);

  content::NotificationService::current()->Notify(
      chrome::NOTIFICATION_TAB_CONTENTS_APPLICATION_EXTENSION_CHANGED,
      content::Source<TabHelper>(this),
      content::NotificationService::NoDetails());
}
