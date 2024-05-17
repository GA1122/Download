content::WebContents* GetTabsAPIDefaultWebContents(
    UIThreadExtensionFunction* function,
    int tab_id,
    std::string* error) {
  content::WebContents* web_contents = nullptr;
  if (tab_id != -1) {
    GetTabById(tab_id, function->browser_context(),
               function->include_incognito_information(),
               nullptr  ,
               nullptr  , &web_contents,
               nullptr  , error);
  } else {
    Browser* browser =
        ChromeExtensionFunctionDetails(function).GetCurrentBrowser();
    if (!browser)
      *error = tabs_constants::kNoCurrentWindowError;
    else if (!ExtensionTabUtil::GetDefaultTab(browser, &web_contents, nullptr))
      *error = tabs_constants::kNoSelectedTabError;
  }
  return web_contents;
}
