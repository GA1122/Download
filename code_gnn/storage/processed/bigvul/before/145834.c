HeadlessDevToolsManagerDelegate::EmulateNetworkConditions(
    content::DevToolsAgentHost* agent_host,
    int session_id,
    int command_id,
    const base::DictionaryValue* params) {
  HeadlessBrowserContextImpl* browser_context =
      static_cast<HeadlessBrowserContextImpl*>(
          browser_->GetDefaultBrowserContext());
  bool offline = false;
  double latency = 0, download_throughput = 0, upload_throughput = 0;
  params->GetBoolean("offline", &offline);
  params->GetDouble("latency", &latency);
  params->GetDouble("downloadThroughput", &download_throughput);
  params->GetDouble("uploadThroughput", &upload_throughput);
  HeadlessNetworkConditions conditions(HeadlessNetworkConditions(
      offline, std::max(latency, 0.0), std::max(download_throughput, 0.0),
      std::max(upload_throughput, 0.0)));
  browser_context->SetNetworkConditions(conditions);
  return CreateSuccessResponse(command_id, nullptr);
}
