bool ExtensionPrefs::GetBrowserActionVisibility(const Extension* extension) {
  const DictionaryValue* extension_prefs = GetExtensionPref(extension->id());
  if (!extension_prefs)
    return true;
  bool visible = false;
  if (!extension_prefs->GetBoolean(kBrowserActionVisible, &visible) || visible)
    return true;

  return false;
}
