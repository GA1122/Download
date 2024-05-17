  bool Get(int64 metahandle, syncable::IndexedBitField field) {
    return GetField(metahandle, field, false);
  }
