bool AppListSyncableService::AppIsOem(const std::string& id) {
  if (!extension_system_->extension_service())
    return false;
  const extensions::Extension* extension =
      extension_system_->extension_service()->GetExtensionById(id, true);
  return extension && extension->was_installed_by_oem();
}
