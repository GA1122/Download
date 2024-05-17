void TestingAutomationProvider::GetNavigationInfo(
    Browser* browser,
    DictionaryValue* args,
    IPC::Message* reply_message) {
  AutomationJSONReply reply(this, reply_message);
  int tab_index;
  WebContents* web_contents = NULL;
  if (!args->GetInteger("tab_index", &tab_index) ||
      !(web_contents = browser->GetWebContentsAt(tab_index))) {
    reply.SendError("tab_index missing or invalid.");
    return;
  }
  scoped_ptr<DictionaryValue> return_value(new DictionaryValue);
  const NavigationController& controller = web_contents->GetController();
  NavigationEntry* nav_entry = controller.GetActiveEntry();
  DCHECK(nav_entry);

  DictionaryValue* ssl = new DictionaryValue;
  std::map<content::SecurityStyle, std::string> style_to_string;
  style_to_string[content::SECURITY_STYLE_UNKNOWN] = "SECURITY_STYLE_UNKNOWN";
  style_to_string[content::SECURITY_STYLE_UNAUTHENTICATED] =
      "SECURITY_STYLE_UNAUTHENTICATED";
  style_to_string[content::SECURITY_STYLE_AUTHENTICATION_BROKEN] =
      "SECURITY_STYLE_AUTHENTICATION_BROKEN";
  style_to_string[content::SECURITY_STYLE_AUTHENTICATED] =
      "SECURITY_STYLE_AUTHENTICATED";

  SSLStatus ssl_status = nav_entry->GetSSL();
  ssl->SetString("security_style",
                 style_to_string[ssl_status.security_style]);
  ssl->SetBoolean("ran_insecure_content",
      !!(ssl_status.content_status & SSLStatus::RAN_INSECURE_CONTENT));
  ssl->SetBoolean("displayed_insecure_content",
      !!(ssl_status.content_status & SSLStatus::DISPLAYED_INSECURE_CONTENT));
  return_value->Set("ssl", ssl);

  std::map<content::PageType, std::string> pagetype_to_string;
  pagetype_to_string[content::PAGE_TYPE_NORMAL] = "NORMAL_PAGE";
  pagetype_to_string[content::PAGE_TYPE_ERROR] = "ERROR_PAGE";
  pagetype_to_string[content::PAGE_TYPE_INTERSTITIAL] =
      "INTERSTITIAL_PAGE";
  return_value->SetString("page_type",
                          pagetype_to_string[nav_entry->GetPageType()]);

  return_value->SetString("favicon_url", nav_entry->GetFavicon().url.spec());
  reply.SendSuccess(return_value.get());
}
