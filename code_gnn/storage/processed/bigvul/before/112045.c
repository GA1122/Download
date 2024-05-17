  int64 Get(int64 metahandle, syncable::Int64Field field) {
    return GetField(metahandle, field, syncable::kInvalidMetaHandle);
  }
