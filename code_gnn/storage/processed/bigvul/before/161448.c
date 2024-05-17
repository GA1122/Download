  void UntrackOriginOnIDBThread(const url::Origin& origin) {
    DCHECK(context_->TaskRunner()->RunsTasksInCurrentSequence());
    origins_.erase(origin);
  }
