  int64 Get(int64 metahandle, syncable::BaseVersion field) {
    const int64 kDefaultValue = -100;
    return GetField(metahandle, field, kDefaultValue);
  }
