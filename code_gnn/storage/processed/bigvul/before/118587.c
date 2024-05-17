base::DictionaryValue* AppLauncherHandler::GetAppInfo(
    const Extension* extension) {
  base::DictionaryValue* app_info = new base::DictionaryValue();
  base::AutoReset<bool> auto_reset(&ignore_changes_, true);
  CreateAppInfo(extension,
                extension_service_,
                app_info);
  return app_info;
}
