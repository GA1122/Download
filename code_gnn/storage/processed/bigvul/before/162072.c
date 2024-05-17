  void DecrementSiteProcessCount(const GURL& site_url,
                                 int render_process_host_id) {
    auto result = map_.find(site_url);
    DCHECK(result != map_.end());
    std::map<ProcessID, Count>& counts_per_process = result->second;

    --counts_per_process[render_process_host_id];
    DCHECK(counts_per_process[render_process_host_id] >= 0);

    if (counts_per_process[render_process_host_id] == 0)
      counts_per_process.erase(render_process_host_id);

    if (counts_per_process.empty())
      map_.erase(site_url);
  }
