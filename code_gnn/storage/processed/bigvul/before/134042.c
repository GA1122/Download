void AppResult::OnExtensionLoaded(content::BrowserContext* browser_context,
                                  const extensions::Extension* extension) {
  UpdateIcon();
}
