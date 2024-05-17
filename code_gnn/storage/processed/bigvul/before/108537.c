  void ScheduleOriginsDeletion() {
    remaining_deleters_ = origins_.size();
    for (std::set<GURL>::const_iterator p = origins_.begin();
         p != origins_.end();
         ++p) {
      OriginDataDeleter* deleter =
          new OriginDataDeleter(
              manager(), *p, type_, quota_client_mask_,
              base::Bind(&HostDataDeleter::DidDeleteOriginData,
                         weak_factory_.GetWeakPtr()));
      deleter->Start();
    }
  }
