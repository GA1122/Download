void Browser::OnInstallApplication(TabContents* source,
                                   const WebApplicationInfo& web_app) {
  ExtensionService* extensions_service = profile()->GetExtensionService();
  if (!extensions_service)
    return;

  scoped_refptr<CrxInstaller> installer(
      new CrxInstaller(extensions_service,
                       extensions_service->show_extensions_prompts() ?
                       new ExtensionInstallUI(profile()) : NULL));
  installer->InstallWebApp(web_app);
}
