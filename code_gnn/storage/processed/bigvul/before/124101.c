    ChromeContentBrowserClient::GetExternalBrowserPpapiHost(
        int plugin_process_id) {
  BrowserChildProcessHostIterator iter(content::PROCESS_TYPE_NACL_LOADER);
  while (!iter.Done()) {
    NaClProcessHost* host = static_cast<NaClProcessHost*>(iter.GetDelegate());
    if (host->process() &&
        host->process()->GetData().id == plugin_process_id) {
      return host->browser_ppapi_host();
    }
    ++iter;
  }
  return NULL;
}
