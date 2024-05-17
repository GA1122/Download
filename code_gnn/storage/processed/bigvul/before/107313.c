TabContents* Browser::OpenApplication(Profile* profile,
                                      const std::string& app_id,
                                      TabContents* existing_tab) {
  ExtensionService* extensions_service = profile->GetExtensionService();

  const Extension* extension =
      extensions_service->GetExtensionById(app_id, false);
  if (!extension)
    return NULL;

  return OpenApplication(profile, extension, extension->launch_container(),
                         existing_tab);
}
