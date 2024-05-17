  Id Get(int64 metahandle, syncable::IdField field) {
    return GetField(metahandle, field, syncable::GetNullId());
  }
