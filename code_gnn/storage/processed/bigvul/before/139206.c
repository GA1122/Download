  void RemoveProcess(RenderProcessHost* host) {
    std::set<std::string> sites;
    for (SiteToProcessMap::const_iterator i = map_.begin(); i != map_.end();
         i++) {
      if (i->second == host)
        sites.insert(i->first);
    }
    for (std::set<std::string>::iterator i = sites.begin(); i != sites.end();
         i++) {
      SiteToProcessMap::iterator iter = map_.find(*i);
      if (iter != map_.end()) {
        DCHECK_EQ(iter->second, host);
        map_.erase(iter);
      }
    }
  }
