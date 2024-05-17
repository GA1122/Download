bool Extension::LoadBackgroundScripts(string16* error) {
  const std::string& key = is_platform_app() ?
      keys::kPlatformAppBackgroundScripts : keys::kBackgroundScripts;
  return LoadBackgroundScripts(key, error);
}
