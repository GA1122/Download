void TestingAutomationProvider::RemoveNTPMostVisitedThumbnail(
    Browser* browser,
    DictionaryValue* args,
    IPC::Message* reply_message) {
  AutomationJSONReply reply(this, reply_message);
  std::string url;
  if (!args->GetString("url", &url)) {
    reply.SendError("Missing or invalid 'url' key.");
    return;
  }
  history::TopSites* top_sites = browser->profile()->GetTopSites();
  if (!top_sites) {
    reply.SendError("TopSites service is not initialized.");
    return;
  }
  top_sites->AddBlacklistedURL(GURL(url));
  reply.SendSuccess(NULL);
}
