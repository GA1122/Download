void ExtensionInstalledBubble::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  if (type == chrome::NOTIFICATION_EXTENSION_LOADED) {
    const Extension* extension =
        content::Details<const Extension>(details).ptr();
    if (extension == extension_) {
      animation_wait_retries_ = 0;
      MessageLoopForUI::current()->PostTask(
          FROM_HERE,
          base::Bind(&ExtensionInstalledBubble::ShowInternal,
                     base::Unretained(this)));
    }
  } else if (type == chrome::NOTIFICATION_EXTENSION_UNLOADED) {
    const Extension* extension =
        content::Details<UnloadedExtensionInfo>(details)->extension;
    if (extension == extension_)
      extension_ = NULL;
  } else {
    NOTREACHED() << L"Received unexpected notification";
  }
}
