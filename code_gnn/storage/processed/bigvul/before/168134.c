  MockAutocompleteHistoryManager* RecreateMockAutocompleteHistoryManager() {
    MockAutocompleteHistoryManager* manager =
        new MockAutocompleteHistoryManager(autofill_driver_.get(),
                                           autofill_manager_->client());
    autofill_manager_->autocomplete_history_manager_.reset(manager);
    return manager;
  }
