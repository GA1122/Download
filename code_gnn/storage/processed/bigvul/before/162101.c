  bool HasProcess(RenderProcessHost* host) const {
    int process_id = host->GetID();
    for (const auto& site_process_id : site_process_set_) {
      if (site_process_id.second == process_id)
        return true;
    }
    return false;
  }
