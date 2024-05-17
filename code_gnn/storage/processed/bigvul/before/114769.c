  bool StartSyncService(const base::Closure& callback,
                        bool will_fail_association) {
    if (sync_service_.get())
      return false;
    SigninManager* signin = SigninManagerFactory::GetForProfile(profile());
    signin->SetAuthenticatedUsername("test_user");
    ProfileSyncComponentsFactoryMock* factory =
        new ProfileSyncComponentsFactoryMock();
    sync_service_.reset(new TestProfileSyncService(
        factory,
        profile(),
        signin,
        ProfileSyncService::AUTO_START,
        false,
        callback));

    SessionDataTypeController *dtc = new SessionDataTypeController(factory,
                                         profile(),
                                         sync_service_.get());
    sync_service_->RegisterDataTypeController(dtc);

    model_associator_ =
        new SessionModelAssociator(sync_service_.get(),
                                   true  );
    change_processor_ = new SessionChangeProcessor(
        dtc, model_associator_,
        true  );
    EXPECT_CALL(*factory, CreateSessionSyncComponents(_, _)).
        WillOnce(Return(ProfileSyncComponentsFactory::SyncComponents(
            model_associator_, change_processor_)));
    EXPECT_CALL(*factory, CreateDataTypeManager(_, _)).
        WillOnce(ReturnNewDataTypeManager());

    TokenServiceFactory::GetForProfile(profile())->IssueAuthTokenForTest(
        GaiaConstants::kSyncService, "token");
    sync_service_->Initialize();
    MessageLoop::current()->Run();
    return true;
  }
