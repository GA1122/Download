void ExtensionSettingsHandler::GetActivePagesForExtensionProcess(
    const std::set<RenderViewHost*>& views,
    std::vector<ExtensionPage> *result) {
  for (std::set<RenderViewHost*>::const_iterator iter = views.begin();
       iter != views.end(); ++iter) {
    RenderViewHost* host = *iter;
    int host_type = host->delegate()->GetRenderViewType();
    if (host == deleting_rvh_ ||
        chrome::VIEW_TYPE_EXTENSION_POPUP == host_type ||
        chrome::VIEW_TYPE_EXTENSION_DIALOG == host_type)
      continue;

    GURL url = host->delegate()->GetURL();
    content::RenderProcessHost* process = host->process();
    result->push_back(
        ExtensionPage(url, process->GetID(), host->routing_id(),
                      process->GetBrowserContext()->IsOffTheRecord()));
  }
}
