  virtual void SetUp() {
    BrowserWithTestWindowTest::SetUp();
    io_thread_.StartIOThread();
    profile()->CreateRequestContext();
    ASSERT_TRUE(temp_dir_.CreateUniqueTempDir());
    registrar_.Add(this, chrome::NOTIFICATION_FOREIGN_SESSION_UPDATED,
        content::NotificationService::AllSources());
    registrar_.Add(this, chrome::NOTIFICATION_SYNC_REFRESH,
        content::NotificationService::AllSources());
  }
