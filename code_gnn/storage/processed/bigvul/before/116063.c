void ExtensionSettingsHandler::HandleEnableIncognitoMessage(
    const ListValue* args) {
  CHECK_EQ(2U, args->GetSize());
  std::string extension_id, enable_str;
  CHECK(args->GetString(0, &extension_id));
  CHECK(args->GetString(1, &enable_str));
  const Extension* extension =
      extension_service_->GetExtensionById(extension_id, true);
  DCHECK(extension);

  AutoReset<bool> auto_reset_ignore_notifications(&ignore_notifications_, true);
  extension_service_->SetIsIncognitoEnabled(extension->id(),
                                            enable_str == "true");
}
