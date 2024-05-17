bool ChromeContentBrowserClient::ShouldUseProcessPerSite(
    content::BrowserContext* browser_context, const GURL& effective_url) {

  Profile* profile = Profile::FromBrowserContext(browser_context);
  if (!profile)
    return false;

  if (chrome::search::ShouldAssignURLToInstantRenderer(effective_url, profile))
    return true;

  if (!effective_url.SchemeIs(extensions::kExtensionScheme))
    return false;

  ExtensionService* extension_service =
      extensions::ExtensionSystem::Get(profile)->extension_service();
  if (!extension_service)
    return false;

  const Extension* extension = extension_service->extensions()->
      GetExtensionOrAppByURL(ExtensionURLInfo(effective_url));
  if (!extension)
    return false;

  if (extension->GetType() == Manifest::TYPE_HOSTED_APP) {
    if (!extension->HasAPIPermission(APIPermission::kBackground) ||
        !extension->allow_background_js_access()) {
      return false;
    }
  }

  return true;
}
