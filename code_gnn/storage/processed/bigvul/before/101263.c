  string Get(int64 metahandle, syncable::StringField field) const {
    return GetField(metahandle, field, string());
  }
