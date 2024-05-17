  explicit ModelObserver(AppListSyncableService* owner)
      : owner_(owner),
        adding_item_(NULL) {
    DVLOG(2) << owner_ << ": ModelObserver Added";
    owner_->GetModel()->AddObserver(this);
  }
