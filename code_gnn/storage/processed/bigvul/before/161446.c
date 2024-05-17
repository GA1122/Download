  void TrackOriginOnIDBThread(const url::Origin& origin) {
    DCHECK(context_->TaskRunner()->RunsTasksInCurrentSequence());
    if (origins_.find(origin) != origins_.end())
      return;
    origins_.insert(origin);
  }
