  explicit RemovePluginDataTester(TestingProfile* profile)
      : helper_(new TestBrowsingDataFlashLSOHelper(profile)) {
    static_cast<ChromeBrowsingDataRemoverDelegate*>(
        profile->GetBrowsingDataRemoverDelegate())
        ->OverrideFlashLSOHelperForTesting(helper_);
  }
