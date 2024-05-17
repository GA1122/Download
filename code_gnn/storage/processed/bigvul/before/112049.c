  bool Get(int64 metahandle, syncable::BitField field) {
    return GetField(metahandle, field, false);
  }
