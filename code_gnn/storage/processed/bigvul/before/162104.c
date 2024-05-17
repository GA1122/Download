  void IncrementSiteProcessCount(const GURL& site_url,
                                 int render_process_host_id) {
    std::map<ProcessID, Count>& counts_per_process = map_[site_url];
    ++counts_per_process[render_process_host_id];

#ifndef NDEBUG
    RenderProcessHost* host = RenderProcessHost::FromID(render_process_host_id);
    if (!HasProcess(host))
      host->AddObserver(this);
#endif
  }
