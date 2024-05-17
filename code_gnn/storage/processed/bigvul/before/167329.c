  void AttachService() {
    const void* kKey = TestingDomainReliabilityServiceFactoryUserData::kKey;

    TestingDomainReliabilityServiceFactoryUserData* data =
        new TestingDomainReliabilityServiceFactoryUserData(profile_,
                                                           mock_service_);
    EXPECT_FALSE(profile_->GetUserData(kKey));
    profile_->SetUserData(kKey, base::WrapUnique(data));

    DomainReliabilityServiceFactory::GetInstance()->SetTestingFactoryAndUse(
        profile_,
        &TestingDomainReliabilityServiceFactoryFunction);

    EXPECT_EQ(data, profile_->GetUserData(kKey));
    EXPECT_TRUE(data->attached);
    profile_->RemoveUserData(kKey);
  }
