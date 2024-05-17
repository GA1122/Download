  bool ProcessedAnyCommands(
      sessions::BaseSessionService* base_session_service) {
    sessions::BaseSessionServiceTestHelper test_helper(base_session_service);
    return test_helper.ProcessedAnyCommands();
  }
