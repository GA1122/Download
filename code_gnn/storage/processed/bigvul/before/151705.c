void Browser::OnExtensionUninstalled(content::BrowserContext* browser_context,
                                     const extensions::Extension* extension,
                                     extensions::UninstallReason reason) {

  if (window() && window()->GetLocationBar())
    window()->GetLocationBar()->UpdatePageActions();
}
