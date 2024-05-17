  virtual void SetUp() {
    web_data_service_ = new MockWebDataService();
    autocomplete_manager_.reset(
        new AutocompleteHistoryManager(&profile_, web_data_service_));
  }
