  static bool VisiblePropertiesDiffer(
      const syncable::EntryKernelMutation& mutation,
      Cryptographer* cryptographer) {
    const syncable::EntryKernel& a = mutation.original;
    const syncable::EntryKernel& b = mutation.mutated;
    const sync_pb::EntitySpecifics& a_specifics = a.ref(SPECIFICS);
    const sync_pb::EntitySpecifics& b_specifics = b.ref(SPECIFICS);
    DCHECK_EQ(syncable::GetModelTypeFromSpecifics(a_specifics),
              syncable::GetModelTypeFromSpecifics(b_specifics));
    syncable::ModelType model_type =
        syncable::GetModelTypeFromSpecifics(b_specifics);
    if (model_type < syncable::FIRST_REAL_MODEL_TYPE ||
        !a.ref(syncable::UNIQUE_SERVER_TAG).empty()) {
      return false;
    }
    if (a.ref(syncable::IS_DIR) != b.ref(syncable::IS_DIR))
      return true;
    if (!AreSpecificsEqual(cryptographer,
                           a.ref(syncable::SPECIFICS),
                           b.ref(syncable::SPECIFICS))) {
      return true;
    }
    if (!a_specifics.has_encrypted() && !b_specifics.has_encrypted() &&
        a.ref(syncable::NON_UNIQUE_NAME) != b.ref(syncable::NON_UNIQUE_NAME))
      return true;
    if (VisiblePositionsDiffer(mutation))
      return true;
    return false;
  }
