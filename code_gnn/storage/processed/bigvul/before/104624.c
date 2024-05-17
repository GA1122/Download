bool Extension::HasFullPermissions() const {
  return permission_set_->HasEffectiveFullAccess();
}
