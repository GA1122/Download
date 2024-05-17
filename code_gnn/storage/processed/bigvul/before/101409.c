  void SyncNotifierUpdateEnabledTypes(
      const syncable::ModelTypeSet& types) {
    ModelSafeRoutingInfo routes;
    GetModelSafeRoutingInfo(&routes);
    syncable::ModelTypeSet expected_types;
    for (ModelSafeRoutingInfo::const_iterator it = routes.begin();
         it != routes.end(); ++it) {
      expected_types.insert(it->first);
    }
    EXPECT_EQ(expected_types, types);
    ++update_enabled_types_call_count_;
  }
