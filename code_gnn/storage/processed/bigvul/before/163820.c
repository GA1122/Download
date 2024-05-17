bool ChromeContentBrowserClientExtensionsPart::ShouldLockToOrigin(
    content::BrowserContext* browser_context,
    const GURL& effective_site_url) {
  if (effective_site_url.SchemeIs(extensions::kExtensionScheme)) {
    const Extension* extension =
        ExtensionRegistry::Get(browser_context)
            ->enabled_extensions()
            .GetExtensionOrAppByURL(effective_site_url);
    if (extension && extension->is_hosted_app())
      return false;

    if (!base::CommandLine::ForCurrentProcess()->HasSwitch(
            ::switches::kSitePerProcess))
      return false;
  }
  return true;
}
