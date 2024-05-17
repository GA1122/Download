void TestingAutomationProvider::MoveNTPMostVisitedThumbnail(
    Browser* browser,
    DictionaryValue* args,
    IPC::Message* reply_message) {
  AutomationJSONReply reply(this, reply_message);
  std::string url, error;
  int index, old_index;
  if (!args->GetString("url", &url)) {
    reply.SendError("Missing or invalid 'url' key.");
    return;
  }
  if (!args->GetInteger("index", &index)) {
    reply.SendError("Missing or invalid 'index' key.");
    return;
  }
  if (!args->GetInteger("old_index", &old_index)) {
    reply.SendError("Missing or invalid 'old_index' key");
    return;
  }
  history::TopSites* top_sites = browser->profile()->GetTopSites();
  if (!top_sites) {
    reply.SendError("TopSites service is not initialized.");
    return;
  }
  GURL swapped;
  if (top_sites->GetPinnedURLAtIndex(index, &swapped)) {
    top_sites->AddPinnedURL(swapped, old_index);
  }
  top_sites->AddPinnedURL(GURL(url), index);
  reply.SendSuccess(NULL);
}
