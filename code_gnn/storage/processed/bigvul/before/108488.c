  void DidGetOriginsForHost(const std::set<GURL>& origins, StorageType type) {
    DCHECK_GT(remaining_clients_, 0);

    origins_.insert(origins.begin(), origins.end());

    if (--remaining_clients_ == 0) {
      if (!origins_.empty())
        ScheduleOriginsDeletion();
      else
        CallCompleted();
    }
  }
