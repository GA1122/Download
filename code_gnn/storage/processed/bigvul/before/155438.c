void ChromeContentBrowserClient::ResourceDispatcherHostCreated() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  safe_browsing_service_ = g_browser_process->safe_browsing_service();

  for (size_t i = 0; i < extra_parts_.size(); ++i)
    extra_parts_[i]->ResourceDispatcherHostCreated();

  return g_browser_process->ResourceDispatcherHostCreated();
}
