  void RunMigrationTest(const MigrationList& migration_list,
                       TriggerMethod trigger_method) {
    ASSERT_TRUE(ShouldRunMigrationTest());

    bool do_test_without_notifications =
        (trigger_method != TRIGGER_NOTIFICATION && num_clients() == 1);

    if (do_test_without_notifications) {
      DisableNotifications();
    }

    for (MigrationList::const_iterator it = migration_list.begin();
         it != migration_list.end(); ++it) {
      TriggerMigrationDoneError(*it);
    }

    for (MigrationList::const_iterator it = migration_list.begin();
         it != migration_list.end(); ++it) {
      TriggerMigration(*it, trigger_method);
      AwaitMigration(*it);
    }

    if (!do_test_without_notifications) {
      AwaitQuiescence();
    }

  }
