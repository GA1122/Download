  void RunTwoClientMigrationTest(const MigrationList& migration_list,
                                    TriggerMethod trigger_method) {
    if (!ShouldRunMigrationTest()) {
      return;
    }
    ASSERT_TRUE(SetupSync());

    VerifyPrefSync();

    RunMigrationTest(migration_list, trigger_method);

    VerifyPrefSync();
  }
