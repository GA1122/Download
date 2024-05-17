MigrationList MakeList(syncable::ModelType type1,
                       syncable::ModelType type2) {
  return MakeList(MakeSet(type1), MakeSet(type2));
}
