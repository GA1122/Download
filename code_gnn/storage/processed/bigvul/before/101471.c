string DirectoryBackingStore::ModelTypeEnumToModelId(ModelType model_type) {
  sync_pb::EntitySpecifics specifics;
  syncable::AddDefaultExtensionValue(model_type, &specifics);
  return specifics.SerializeAsString();
}
