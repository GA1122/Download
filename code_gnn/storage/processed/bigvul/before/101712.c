void Browser::OnInstallApplication(TabContentsWrapper* source,
                                   const WebApplicationInfo& web_app) {
  ExtensionService* extension_service = profile()->GetExtensionService();
  if (!extension_service)
    return;

  scoped_refptr<CrxInstaller> installer(extension_service->MakeCrxInstaller(
      extension_service->show_extensions_prompts() ?
      new ExtensionInstallUI(profile()) : NULL));
  installer->InstallWebApp(web_app);
}
