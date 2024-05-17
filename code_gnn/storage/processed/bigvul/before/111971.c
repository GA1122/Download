  void TriggerMigration(syncable::ModelTypeSet model_types,
                        TriggerMethod trigger_method) {
    switch (trigger_method) {
      case MODIFY_PREF:
        ASSERT_EQ(1, num_clients());
        ASSERT_TRUE(BooleanPrefMatches(prefs::kShowHomeButton));
        ChangeBooleanPref(0, prefs::kShowHomeButton);
        break;
      case MODIFY_BOOKMARK:
        ASSERT_TRUE(AddURL(0, IndexedURLTitle(0), GURL(IndexedURL(0))));
        break;
      case TRIGGER_NOTIFICATION:
        TriggerNotification(model_types);
        break;
      default:
        ADD_FAILURE();
    }
  }
