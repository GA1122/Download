const Extension* ExtensionContextMenuModel::GetExtension() const {
  ExtensionService* extension_service = profile_->GetExtensionService();
  return extension_service->GetExtensionById(extension_id_, false);
}
