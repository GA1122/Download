bool ExtensionService::UninstallExtension(
    const std::string& extension_id_unsafe,
    bool external_uninstall,
    std::string* error) {
  CHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  std::string extension_id(extension_id_unsafe);

  const Extension* extension = GetInstalledExtension(extension_id);

  CHECK(extension);

  GURL extension_url(extension->url());
  Extension::Location location(extension->location());

  if (!Extension::UserMayDisable(location) && !external_uninstall) {
    NotificationService::current()->Notify(
        NotificationType::EXTENSION_UNINSTALL_NOT_ALLOWED,
        Source<Profile>(profile_),
        Details<const Extension>(extension));
    if (error != NULL) {
      *error = errors::kCannotUninstallManagedExtension;
    }
    return false;
  }

  UninstalledExtensionInfo uninstalled_extension_info(*extension);

  UMA_HISTOGRAM_ENUMERATION("Extensions.UninstallType",
                            extension->GetType(), 100);
  RecordPermissionMessagesHistogram(
      extension, "Extensions.Permissions_Uninstall");

  if (profile_->GetTemplateURLModel())
    profile_->GetTemplateURLModel()->UnregisterExtensionKeyword(extension);

  UnloadExtension(extension_id, UnloadedExtensionInfo::UNINSTALL);

  extension_prefs_->OnExtensionUninstalled(extension_id, location,
                                           external_uninstall);

  if (Extension::LOAD != location) {
    if (!BrowserThread::PostTask(
            BrowserThread::FILE, FROM_HERE,
            NewRunnableFunction(
                &extension_file_util::UninstallExtension,
                install_directory_,
                extension_id)))
      NOTREACHED();
  }

  ClearExtensionData(extension_url);
  UntrackTerminatedExtension(extension_id);

  NotificationService::current()->Notify(
      NotificationType::EXTENSION_UNINSTALLED,
      Source<Profile>(profile_),
      Details<UninstalledExtensionInfo>(&uninstalled_extension_info));

  return true;
}
