  MigrationList GetPreferredDataTypesList() {
    MigrationList migration_list;
    const syncable::ModelTypeSet preferred_data_types =
        GetPreferredDataTypes();
    for (syncable::ModelTypeSet::Iterator it =
             preferred_data_types.First(); it.Good(); it.Inc()) {
      migration_list.push_back(MakeSet(it.Get()));
    }
    return migration_list;
  }
