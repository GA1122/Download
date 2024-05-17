void TabHelper::DidNavigateMainFrame(
    const content::LoadCommittedDetails& details,
    const content::FrameNavigateParams& params) {
  InvokeForContentRulesRegistries(
      [this, &details, &params](ContentRulesRegistry* registry) {
    registry->DidNavigateMainFrame(web_contents(), details, params);
  });

  content::BrowserContext* context = web_contents()->GetBrowserContext();
  ExtensionRegistry* registry = ExtensionRegistry::Get(context);
  const ExtensionSet& enabled_extensions = registry->enabled_extensions();

  if (util::IsNewBookmarkAppsEnabled()) {
    Browser* browser = chrome::FindBrowserWithWebContents(web_contents());
    if (browser && browser->is_app()) {
      const Extension* extension = registry->GetExtensionById(
          web_app::GetExtensionIdFromApplicationName(browser->app_name()),
          ExtensionRegistry::EVERYTHING);
      if (extension && AppLaunchInfo::GetFullLaunchURL(extension).is_valid())
        SetExtensionApp(extension);
    } else {
      UpdateExtensionAppIcon(
          enabled_extensions.GetExtensionOrAppByURL(params.url));
    }
  } else {
    UpdateExtensionAppIcon(
        enabled_extensions.GetExtensionOrAppByURL(params.url));
  }

  if (!details.is_in_page)
    ExtensionActionAPI::Get(context)->ClearAllValuesForTab(web_contents());
}
