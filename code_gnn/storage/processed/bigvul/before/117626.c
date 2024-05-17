bool GetAppLaunchContainer(
    Profile* profile,
    const std::string& app_id,
    const Extension** out_extension,
    extension_misc::LaunchContainer* out_launch_container) {

  ExtensionService* extensions_service = profile->GetExtensionService();
  const Extension* extension =
      extensions_service->GetExtensionById(app_id, false);

  if (!extension)
    return false;

  extension_misc::LaunchContainer launch_container =
      extensions_service->extension_prefs()->GetLaunchContainer(
          extension, ExtensionPrefs::LAUNCH_WINDOW);

  *out_extension = extension;
  *out_launch_container = launch_container;
  return true;
}
