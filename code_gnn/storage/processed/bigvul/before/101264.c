  int64 Get(int64 metahandle, syncable::Int64Field field) const {
    return GetField(metahandle, field, syncable::kInvalidMetaHandle);
  }
