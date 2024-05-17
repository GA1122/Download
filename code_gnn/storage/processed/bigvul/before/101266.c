  bool Get(int64 metahandle, syncable::IndexedBitField field) const {
    return GetField(metahandle, field, false);
  }
