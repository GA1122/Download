void ExtensionSettingsHandler::HandleOptionsMessage(const ListValue* args) {
  const Extension* extension = GetExtension(args);
  if (!extension || extension->options_url().is_empty())
    return;
  Profile::FromWebUI(web_ui())->GetExtensionProcessManager()->OpenOptionsPage(
      extension, NULL);
}
