void ExtensionSettingsHandler::HandleAllowFileAccessMessage(
    const ListValue* args) {
  CHECK_EQ(2U, args->GetSize());
  std::string extension_id, allow_str;
  CHECK(args->GetString(0, &extension_id));
  CHECK(args->GetString(1, &allow_str));
  const Extension* extension =
      extension_service_->GetExtensionById(extension_id, true);
  DCHECK(extension);

  if (!Extension::UserMayDisable(extension->location())) {
    LOG(ERROR) << "Attempt to change allow file access of an extension that is "
               << "non-usermanagable was made. Extension id : "
               << extension->id();
    return;
  }

  extension_service_->SetAllowFileAccess(extension, allow_str == "true");
}
