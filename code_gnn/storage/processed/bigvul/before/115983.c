void ExtensionContextMenuModel::ExtensionUninstallAccepted() {
  if (GetExtension())
    profile_->GetExtensionService()->UninstallExtension(extension_id_, false,
                                                        NULL);

  Release();
}
