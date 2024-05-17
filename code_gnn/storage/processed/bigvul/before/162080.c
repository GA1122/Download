  RenderProcessHost* FindFreshestProcessForSite(const GURL& site_url) const {
    for (const auto& site_process_pair : base::Reversed(site_process_set_)) {
      if (site_process_pair.first == site_url)
        return RenderProcessHost::FromID(site_process_pair.second);
    }
    return nullptr;
  }
