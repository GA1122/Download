  virtual ~FakeDownloadItem() {
    FOR_EACH_OBSERVER(Observer, observers_, OnDownloadDestroyed(this));
    EXPECT_EQ(0u, observers_.size());
  }
