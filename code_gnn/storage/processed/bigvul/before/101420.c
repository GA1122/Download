  bool StartSyncService(Task* task, bool will_fail_association) {
    if (sync_service_.get())
      return false;
    sync_service_.reset(new TestProfileSyncService(
        &factory_, profile(), "test user", false, task));
    SessionServiceFactory::SetForTestProfile(profile(), helper_.service());

    model_associator_ =
        new SessionModelAssociator(sync_service_.get(),
                                   true  );
    change_processor_ = new SessionChangeProcessor(
        sync_service_.get(), model_associator_,
        true  );
    EXPECT_CALL(factory_, CreateSessionSyncComponents(_, _)).
        WillOnce(Return(ProfileSyncFactory::SyncComponents(
            model_associator_, change_processor_)));
    EXPECT_CALL(factory_, CreateDataTypeManager(_, _)).
        WillOnce(ReturnNewDataTypeManager());
    sync_service_->RegisterDataTypeController(
        new SessionDataTypeController(&factory_,
                                      profile(),
                                      sync_service_.get()));
    profile()->GetTokenService()->IssueAuthTokenForTest(
        GaiaConstants::kSyncService, "token");
    sync_service_->Initialize();
    MessageLoop::current()->Run();
    return true;
  }
