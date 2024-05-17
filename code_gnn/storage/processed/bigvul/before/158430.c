  void ReclaimResources(const std::vector<viz::ReturnedResource>& resources) {
    GetDelegatedFrameHost()->ReclaimResources(resources);
  }
