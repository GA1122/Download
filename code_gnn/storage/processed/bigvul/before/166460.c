  explicit FakeDownloadProtectionService(
      const SafeBrowsingTestConfiguration* test_config)
      : DownloadProtectionService(nullptr), test_configuration_(test_config) {}
