  bool Get(int64 metahandle, syncable::BitField field) const {
    return GetField(metahandle, field, false);
  }
