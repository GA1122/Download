  void SyncNotifierUpdateEnabledTypes(syncable::ModelTypeSet types) {
    ModelSafeRoutingInfo routes;
    GetModelSafeRoutingInfo(&routes);
    const syncable::ModelTypeSet expected_types =
        GetRoutingInfoTypes(routes);
    EXPECT_TRUE(types.Equals(expected_types));
    ++update_enabled_types_call_count_;
  }
