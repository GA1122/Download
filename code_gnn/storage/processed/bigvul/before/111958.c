MigrationList MakeList(syncable::ModelTypeSet model_types1,
                       syncable::ModelTypeSet model_types2) {
  MigrationList migration_list;
  migration_list.push_back(model_types1);
  migration_list.push_back(model_types2);
  return migration_list;
}
