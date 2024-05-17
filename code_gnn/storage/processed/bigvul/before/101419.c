  virtual void SetUp() {
    BrowserWithTestWindowTest::SetUp();
    base::Thread::Options options;
    options.message_loop_type = MessageLoop::TYPE_IO;
    io_thread_.StartWithOptions(options);
    profile()->CreateRequestContext();
    ASSERT_TRUE(temp_dir_.CreateUniqueTempDir());
    SessionService* session_service = new SessionService(temp_dir_.path());
    helper_.set_service(session_service);
    service()->SetWindowType(window_id_, Browser::TYPE_TABBED);
    service()->SetWindowBounds(window_id_,
                               window_bounds_,
                               ui::SHOW_STATE_NORMAL);
    registrar_.Add(this, chrome::NOTIFICATION_FOREIGN_SESSION_UPDATED,
        NotificationService::AllSources());
  }
