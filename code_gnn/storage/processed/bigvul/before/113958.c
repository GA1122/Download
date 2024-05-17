  explicit FakeRegistrationManager(
      invalidation::InvalidationClient* invalidation_client)
      : RegistrationManager(invalidation_client),
        jitter_(0.0) {}
