void TestingAutomationProvider::RestoreAllNTPMostVisitedThumbnails(
    Browser* browser,
    DictionaryValue* args,
    IPC::Message* reply_message) {
  AutomationJSONReply reply(this, reply_message);
  history::TopSites* top_sites = browser->profile()->GetTopSites();
  if (!top_sites) {
    reply.SendError("TopSites service is not initialized.");
    return;
  }
  top_sites->ClearBlacklistedURLs();
  reply.SendSuccess(NULL);
}
