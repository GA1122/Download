void BrowserEventRouter::ScriptBadgeExecuted(
    Profile* profile,
    const ExtensionAction& script_badge,
    int tab_id) {
  WebContents* web_contents = NULL;
  if (!ExtensionTabUtil::GetTabById(tab_id, profile, profile->IsOffTheRecord(),
                                    NULL, NULL, &web_contents, NULL)) {
    return;
  }
  ExtensionActionExecuted(profile, script_badge, web_contents);
}
