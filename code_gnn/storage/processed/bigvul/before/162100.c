  bool HasProcess(RenderProcessHost* process) {
    for (auto iter : map_) {
      std::map<ProcessID, Count>& counts_per_process = iter.second;
      for (auto iter_process : counts_per_process) {
        if (iter_process.first == process->GetID())
          return true;
      }
    }
    return false;
  }
