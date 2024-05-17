const Extension* AutomationProvider::GetEnabledExtension(int extension_handle) {
  const Extension* extension =
      extension_tracker_->GetResource(extension_handle);
  ExtensionService* service = profile_->GetExtensionService();
  if (extension && service &&
      service->GetExtensionById(extension->id(), false))
    return extension;
  return NULL;
}
