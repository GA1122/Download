  explicit RemovePermissionPromptCountsTest(TestingProfile* profile)
      : autoblocker_(PermissionDecisionAutoBlocker::GetForProfile(profile)) {}
