void TestingAutomationProvider::NavigateToURL(
    DictionaryValue* args,
    IPC::Message* reply_message) {
  if (SendErrorIfModalDialogActive(this, reply_message))
    return;

  int navigation_count;
  std::string url, error;
  Browser* browser;
  WebContents* web_contents;
  if (!GetBrowserAndTabFromJSONArgs(args, &browser, &web_contents, &error)) {
    AutomationJSONReply(this, reply_message).SendError(error);
    return;
  }
  if (!args->GetString("url", &url)) {
    AutomationJSONReply(this, reply_message)
        .SendError("'url' missing or invalid");
    return;
  }
  if (!args->GetInteger("navigation_count", &navigation_count)) {
    AutomationJSONReply(this, reply_message)
        .SendError("'navigation_count' missing or invalid");
    return;
  }
  new NavigationNotificationObserver(
      &web_contents->GetController(), this, reply_message,
      navigation_count, false, true);
  browser->OpenURLFromTab(web_contents, OpenURLParams(
      GURL(url), content::Referrer(), CURRENT_TAB,
      content::PAGE_TRANSITION_TYPED, false));
}
