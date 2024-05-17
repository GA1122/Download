  virtual MalwareDetails* CreateMalwareDetails(
      SafeBrowsingService* sb_service,
      TabContents* tab_contents,
      const SafeBrowsingService::UnsafeResource& unsafe_resource) {
    details_ = new FakeMalwareDetails(sb_service, tab_contents,
                                      unsafe_resource);
    return details_;
  }
