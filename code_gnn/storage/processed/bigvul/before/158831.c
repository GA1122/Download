MaybeCreateHostedAppController(Browser* browser) {
#if BUILDFLAG(ENABLE_EXTENSIONS)
  const std::string extension_id =
      web_app::GetAppIdFromApplicationName(browser->app_name());
  const Extension* extension =
      extensions::ExtensionRegistry::Get(browser->profile())
          ->GetExtensionById(extension_id,
                             extensions::ExtensionRegistry::EVERYTHING);
  if (extension && extension->is_hosted_app())
    return std::make_unique<extensions::HostedAppBrowserController>(browser);
#endif
  return nullptr;
}
