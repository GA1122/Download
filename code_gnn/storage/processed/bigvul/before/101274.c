  virtual void SetUp() {
    syncdb_.SetUp();

    mock_server_.reset(
        new MockConnectionManager(syncdb_.manager(), syncdb_.name()));
    EnableDatatype(syncable::BOOKMARKS);
    worker_ = new ModelSafeWorker();
    std::vector<SyncEngineEventListener*> listeners;
    listeners.push_back(this);
    context_.reset(new SyncSessionContext(mock_server_.get(),
        syncdb_.manager(), this, listeners));
    context_->set_account_name(syncdb_.name());
    ASSERT_FALSE(context_->resolver());
    syncer_ = new Syncer();
    session_.reset(MakeSession());

    ScopedDirLookup dir(syncdb_.manager(), syncdb_.name());
    CHECK(dir.good());
    ReadTransaction trans(FROM_HERE, dir);
    syncable::Directory::ChildHandles children;
    dir->GetChildHandlesById(&trans, trans.root_id(), &children);
    ASSERT_TRUE(0 == children.size());
    saw_syncer_event_ = false;
    root_id_ = TestIdFactory::root();
    parent_id_ = ids_.MakeServer("parent id");
    child_id_ = ids_.MakeServer("child id");
  }
