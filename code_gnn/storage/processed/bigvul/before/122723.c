bool Extension::ShouldSkipPermissionWarnings() const {
  return IsTrustedId(id());
}
