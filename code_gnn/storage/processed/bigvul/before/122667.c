bool Extension::IsSyncable() const {

  bool is_syncable = (location() == Manifest::INTERNAL &&
      !was_installed_by_default());
  is_syncable |= (id() ==  extension_misc::kWebStoreAppId);
  return is_syncable;
}
