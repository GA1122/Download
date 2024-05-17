bool Extension::CanSilentlyIncreasePermissions() const {
  return location() != Manifest::INTERNAL;
}
