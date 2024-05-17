  bool loading() {
    if (test_data_providers.empty())
      return false;
    return GetProvider()->loading();
  }
