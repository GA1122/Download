void ExtensionAppItem::Reload() {
  const Extension* extension = GetExtension();
  bool is_installing = !extension;
  SetIsInstalling(is_installing);
  if (is_installing) {
    SetName(extension_name_);
    UpdateIcon();
    return;
  }
  SetNameAndShortName(extension->name(), extension->short_name());
  LoadImage(extension);
}
