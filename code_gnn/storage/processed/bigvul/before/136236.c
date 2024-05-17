bool CSPHandler::AlwaysParseForType(Manifest::Type type) const {
  if (is_platform_app_)
    return type == Manifest::TYPE_PLATFORM_APP;
  else
    return type == Manifest::TYPE_EXTENSION ||
        type == Manifest::TYPE_LEGACY_PACKAGED_APP;
}
