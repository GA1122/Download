  void RegisterProcess(const std::string& site, RenderProcessHost* process) {
    SiteToProcessMap::iterator i = map_.find(site);
    if (i == map_.end())
      map_[site] = process;
  }
