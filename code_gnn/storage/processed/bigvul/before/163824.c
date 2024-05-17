bool ChromeContentBrowserClientExtensionsPart::ShouldUseProcessPerSite(
    Profile* profile, const GURL& effective_url) {
  if (!effective_url.SchemeIs(kExtensionScheme))
    return false;

  ExtensionRegistry* registry = ExtensionRegistry::Get(profile);
  if (!registry)
    return false;

  const Extension* extension =
      registry->enabled_extensions().GetByID(effective_url.host());
  if (!extension)
    return false;

  if (extension->GetType() == Manifest::TYPE_HOSTED_APP) {
    if (!extension->permissions_data()->HasAPIPermission(
            APIPermission::kBackground) ||
        !BackgroundInfo::AllowJSAccess(extension)) {
      return false;
    }
  }

  return true;
}
