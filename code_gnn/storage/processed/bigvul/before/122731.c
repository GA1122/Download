bool Extension::can_be_incognito_enabled() const {
  return !is_platform_app();
}
