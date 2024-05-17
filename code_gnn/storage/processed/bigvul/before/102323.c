void ExtensionPrefs::SetBrowserActionVisibility(const Extension* extension,
                                                bool visible) {
  if (GetBrowserActionVisibility(extension) == visible)
    return;

  UpdateExtensionPref(extension->id(), kBrowserActionVisible,
                      Value::CreateBooleanValue(visible));
  content::NotificationService::current()->Notify(
      chrome::NOTIFICATION_EXTENSION_BROWSER_ACTION_VISIBILITY_CHANGED,
      content::Source<ExtensionPrefs>(this),
      content::Details<const Extension>(extension));
}
