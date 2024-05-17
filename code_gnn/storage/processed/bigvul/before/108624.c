  HTTPSOCSPTest()
      : context_(true),
        ev_test_policy_(
            new ScopedTestEVPolicy(EVRootCAMetadata::GetInstance(),
                                   kOCSPTestCertFingerprint,
                                   kOCSPTestCertPolicy)) {
  }
