content::WebContents* ZoomAPIFunction::GetWebContents(int tab_id) {
  content::WebContents* web_contents = NULL;
  if (tab_id != -1) {
    GetTabById(tab_id, browser_context(), include_incognito(),
               nullptr  ,
               nullptr  , &web_contents,
               nullptr  , &error_);
  } else {
    Browser* browser = ChromeExtensionFunctionDetails(this).GetCurrentBrowser();
    if (!browser)
      error_ = keys::kNoCurrentWindowError;
    else if (!ExtensionTabUtil::GetDefaultTab(browser, &web_contents, NULL))
      error_ = keys::kNoSelectedTabError;
  }
  return web_contents;
}
