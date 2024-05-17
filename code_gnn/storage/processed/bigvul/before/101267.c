  bool Get(int64 metahandle, syncable::IsDelField field) const {
    return GetField(metahandle, field, false);
  }
