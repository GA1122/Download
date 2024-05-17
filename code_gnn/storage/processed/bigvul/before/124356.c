void BrowserEventRouter::BrowserActionExecuted(
    const ExtensionAction& browser_action,
    Browser* browser) {
  Profile* profile = browser->profile();
  WebContents* web_contents = NULL;
  int tab_id = 0;
  if (!ExtensionTabUtil::GetDefaultTab(browser, &web_contents, &tab_id))
    return;
  ExtensionActionExecuted(profile, browser_action, web_contents);
}
