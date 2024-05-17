  void CreateRootCallback(ProfileSyncServiceSessionTest* test) {
    success_ = ProfileSyncServiceTestHelper::CreateRoot(
        syncable::SESSIONS, test->sync_service()->GetUserShare(), test->ids());
  }
