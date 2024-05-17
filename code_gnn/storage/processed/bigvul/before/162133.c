  void RegisterProcessForSite(RenderProcessHost* host, const GURL& site_url) {
    if (!HasProcess(host))
      host->AddObserver(this);
    site_process_set_.insert(SiteProcessIDPair(site_url, host->GetID()));
  }
