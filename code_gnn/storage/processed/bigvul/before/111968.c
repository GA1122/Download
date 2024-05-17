  void RunSingleClientMigrationTest(const MigrationList& migration_list,
                                    TriggerMethod trigger_method) {
    if (!ShouldRunMigrationTest()) {
      return;
    }
    ASSERT_TRUE(SetupSync());
    RunMigrationTest(migration_list, trigger_method);
  }
