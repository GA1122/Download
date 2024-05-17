bool IsSyncable(const Extension* extension) {

  bool is_syncable = (extension->location() == Manifest::INTERNAL &&
                      !extension->was_installed_by_default());
  is_syncable |= (extension->id() == extension_misc::kWebStoreAppId);
  return is_syncable;
}
