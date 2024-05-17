bool RenderProcessHostImpl::IsSuitableHost(
    RenderProcessHost* host,
    BrowserContext* browser_context,
    const GURL& site_url) {
  if (run_renderer_in_process())
    return true;

  if (host->GetBrowserContext() != browser_context)
    return false;

  StoragePartition* dest_partition =
      BrowserContext::GetStoragePartitionForSite(browser_context, site_url);
  if (!host->InSameStoragePartition(dest_partition))
    return false;

  if (host->IsGuest())
    return true;

  if (!host->IsGuest() && site_url.SchemeIs(chrome::kGuestScheme))
    return false;

  if (ChildProcessSecurityPolicyImpl::GetInstance()->HasWebUIBindings(
          host->GetID()) !=
      WebUIControllerFactoryRegistry::GetInstance()->UseWebUIBindingsForURL(
          browser_context, site_url)) {
    return false;
  }

  return GetContentClient()->browser()->IsSuitableHost(host, site_url);
}
