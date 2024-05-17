void ExtensionDevToolsClientHost::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  DCHECK(type == chrome::NOTIFICATION_EXTENSION_UNLOADED);
  std::string id =
      content::Details<UnloadedExtensionInfo>(details)->extension->id();
  if (id == extension_id_)
      Close();
}
