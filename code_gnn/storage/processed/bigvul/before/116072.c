void ExtensionSettingsHandler::HandleUninstallMessage(const ListValue* args) {
  std::string extension_id = UTF16ToUTF8(ExtractStringValue(args));
  CHECK(!extension_id.empty());
  const Extension* extension =
      extension_service_->GetExtensionById(extension_id, true);
  if (!extension)
    extension = extension_service_->GetTerminatedExtension(extension_id);
  if (!extension)
    return;

  if (!Extension::UserMayDisable(extension->location())) {
    LOG(ERROR) << "Attempt to uninstall an extension that is non-usermanagable "
               << "was made. Extension id : " << extension->id();
    return;
  }

  if (!extension_id_prompting_.empty())
    return;   

  extension_id_prompting_ = extension_id;

  GetExtensionUninstallDialog()->ConfirmUninstall(extension);
}
