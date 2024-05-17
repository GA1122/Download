  TestVariationsService(web_resource::TestRequestAllowedNotifier* test_notifier,
                        PrefService* local_state)
      : VariationsService(test_notifier, local_state, NULL),
        intercepts_fetch_(true),
        fetch_attempted_(false),
        seed_stored_(false) {
    SetCreateTrialsFromSeedCalledForTesting(true);
  }
