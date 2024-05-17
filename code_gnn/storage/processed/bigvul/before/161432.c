  void AddObserverOnIDBThread() {
    DCHECK(context_->TaskRunner()->RunsTasksInCurrentSequence());
    context_->AddObserver(this);
  }
