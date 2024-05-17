bool ExtensionHasLockedFullscreenPermission(const Extension* extension) {
  return extension->permissions_data()->HasAPIPermission(
      APIPermission::kLockWindowFullscreenPrivate);
}
