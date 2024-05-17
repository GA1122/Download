ExtensionFunction::ResponseAction TabsReloadFunction::Run() {
  std::unique_ptr<tabs::Reload::Params> params(
      tabs::Reload::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  bool bypass_cache = false;
  if (params->reload_properties.get() &&
      params->reload_properties->bypass_cache.get()) {
    bypass_cache = *params->reload_properties->bypass_cache;
  }

  content::WebContents* web_contents = NULL;

  Browser* current_browser =
      ChromeExtensionFunctionDetails(this).GetCurrentBrowser();
  if (!params->tab_id.get()) {
    if (!current_browser)
      return RespondNow(Error(tabs_constants::kNoCurrentWindowError));

    if (!ExtensionTabUtil::GetDefaultTab(current_browser, &web_contents, NULL))
      return RespondNow(Error(kUnknownErrorDoNotUse));
  } else {
    int tab_id = *params->tab_id;

    Browser* browser = NULL;
    std::string error;
    if (!GetTabById(tab_id, browser_context(), include_incognito_information(),
                    &browser, NULL, &web_contents, NULL, &error)) {
      return RespondNow(Error(error));
    }
  }

  if (web_contents->ShowingInterstitialPage()) {
    NavigationEntry* entry = web_contents->GetController().GetVisibleEntry();
    GURL reload_url = entry ? entry->GetURL() : GURL(url::kAboutBlankURL);
    OpenURLParams params(reload_url, Referrer(),
                         WindowOpenDisposition::CURRENT_TAB,
                         ui::PAGE_TRANSITION_RELOAD, false);
    current_browser->OpenURL(params);
  } else {
    web_contents->GetController().Reload(
        bypass_cache ? content::ReloadType::BYPASSING_CACHE
                     : content::ReloadType::NORMAL,
        true);
  }

  return RespondNow(NoArguments());
}
