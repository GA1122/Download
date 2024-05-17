GURL ChromeContentBrowserClient::GetEffectiveURL(
    content::BrowserContext* browser_context, const GURL& url) {
  Profile* profile = Profile::FromBrowserContext(browser_context);
  if (!profile)
    return url;

  if (chrome::search::ShouldAssignURLToInstantRenderer(url, profile))
    return GetEffectiveURLForInstant(url, profile);

  ExtensionService* extension_service =
      extensions::ExtensionSystem::Get(profile)->extension_service();
  if (!extension_service)
    return url;

  const Extension* extension = extension_service->extensions()->
      GetHostedAppByURL(ExtensionURLInfo(url));
  if (!extension)
    return url;

  if (extension->from_bookmark())
    return url;

  return extension->GetResourceURL(url.path());
}
