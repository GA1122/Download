void ChromeContentBrowserClientExtensionsPart::OverrideNavigationParams(
    content::SiteInstance* site_instance,
    ui::PageTransition* transition,
    bool* is_renderer_initiated,
    content::Referrer* referrer) {
  const Extension* extension =
      ExtensionRegistry::Get(site_instance->GetBrowserContext())
          ->enabled_extensions()
          .GetExtensionOrAppByURL(site_instance->GetSiteURL());
  if (!extension)
    return;

  if (extension->id() == extension_misc::kBookmarkManagerId &&
      ui::PageTransitionCoreTypeIs(*transition, ui::PAGE_TRANSITION_LINK)) {
    *transition = ui::PAGE_TRANSITION_AUTO_BOOKMARK;
    *is_renderer_initiated = false;
  }

  if (extension->is_extension())
    *referrer = content::Referrer();
}
