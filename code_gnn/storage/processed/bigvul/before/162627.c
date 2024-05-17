HeadlessDevToolsManagerDelegate::EmulateNetworkConditions(
    content::DevToolsAgentHost* agent_host,
    int session_id,
    int command_id,
    const base::DictionaryValue* params) {
  std::vector<HeadlessBrowserContext*> browser_contexts =
      browser_->GetAllBrowserContexts();
  if (browser_contexts.empty())
    return CreateSuccessResponse(command_id, nullptr);
  const base::Value* offline_value = params->FindKey("offline");
  const base::Value* latency_value = params->FindKey("latency");
  const base::Value* download_throughput_value =
      params->FindKey("downloadThroughput");
  const base::Value* upload_throughput_value =
      params->FindKey("uploadThroughput");
  HeadlessNetworkConditions conditions(HeadlessNetworkConditions(
      offline_value ? offline_value->GetBool() : false,
      latency_value ? std::max(latency_value->GetDouble(), 0.0) : 0,
      download_throughput_value
          ? std::max(download_throughput_value->GetDouble(), 0.0)
          : 0,
      upload_throughput_value
          ? std::max(upload_throughput_value->GetDouble(), 0.0)
          : 0));
  SetNetworkConditions(browser_contexts, conditions);
  return CreateSuccessResponse(command_id, nullptr);
}
