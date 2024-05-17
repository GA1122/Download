RenderProcessHost* RenderProcessHost::GetExistingProcessHost(
    BrowserContext* browser_context,
    const GURL& site_url) {
  std::vector<RenderProcessHost*> suitable_renderers;
  suitable_renderers.reserve(g_all_hosts.Get().size());

  iterator iter(AllHostsIterator());
  while (!iter.IsAtEnd()) {
    if (RenderProcessHostImpl::IsSuitableHost(
            iter.GetCurrentValue(),
            browser_context, site_url))
      suitable_renderers.push_back(iter.GetCurrentValue());

    iter.Advance();
  }

  if (!suitable_renderers.empty()) {
    int suitable_count = static_cast<int>(suitable_renderers.size());
    int random_index = base::RandInt(0, suitable_count - 1);
    return suitable_renderers[random_index];
  }

  return NULL;
}