void ExtensionSettingsHandler::HandleShowButtonMessage(const ListValue* args) {
  const Extension* extension = GetExtension(args);
  extension_service_->SetBrowserActionVisibility(extension, true);
}
