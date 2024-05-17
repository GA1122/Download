void ChromeContentBrowserClient::PluginProcessHostCreated(
    PluginProcessHost* host) {
  host->AddFilter(new ChromePluginMessageFilter(host));
}
