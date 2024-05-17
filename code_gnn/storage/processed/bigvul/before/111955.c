  syncable::ModelTypeSet GetPreferredDataTypes() {
    const syncable::ModelTypeSet preferred_data_types =
        GetClient(0)->service()->GetPreferredDataTypes();
    for (int i = 1; i < num_clients(); ++i) {
      const syncable::ModelTypeSet other_preferred_data_types =
          GetClient(i)->service()->GetPreferredDataTypes();
      EXPECT_TRUE(preferred_data_types.Equals(other_preferred_data_types));
    }
    return preferred_data_types;
  }
