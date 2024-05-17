  explicit ClearDomainReliabilityTester(TestingProfile* profile) :
      profile_(profile),
      mock_service_(new MockDomainReliabilityService()) {
    AttachService();
  }
