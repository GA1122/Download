bool PermissionsData::AllUrlsIncludesChromeUrls(
    const std::string& extension_id) {
  return extension_id == extension_misc::kChromeVoxExtensionId;
}
