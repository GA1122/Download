  virtual void SetUp() {
    context_.reset(new SyncSessionContext(NULL, NULL, this,
        std::vector<SyncEngineEventListener*>()));
    routes_.clear();
    routes_[syncable::BOOKMARKS] = GROUP_UI;
    routes_[syncable::AUTOFILL] = GROUP_UI;
    session_.reset(MakeSession());
  }
