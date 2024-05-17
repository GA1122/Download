scoped_ptr<ActionInfo> LoadExtensionActionInfoHelper(
    const Extension* extension,
    const DictionaryValue* extension_action,
    string16* error) {
  return manifest_handler_helpers::LoadActionInfo(
      extension, extension_action, error);
}
