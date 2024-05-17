    ShouldFrameShareParentSiteInstanceDespiteTopDocumentIsolation(
        const GURL& subframe_url,
        content::SiteInstance* parent_site_instance) {
  const Extension* extension =
      ExtensionRegistry::Get(parent_site_instance->GetBrowserContext())
          ->enabled_extensions()
          .GetExtensionOrAppByURL(parent_site_instance->GetSiteURL());

  return extension && extension->is_hosted_app();
}
