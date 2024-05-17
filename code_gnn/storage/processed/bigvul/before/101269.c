  ValueType GetField(int64 metahandle, FieldType field,
      ValueType default_value) const {
    ScopedDirLookup dir(syncdb_.manager(), syncdb_.name());
    EXPECT_TRUE(dir.good());
    ReadTransaction trans(FROM_HERE, dir);
    Entry entry(&trans, GET_BY_HANDLE, metahandle);
    EXPECT_TRUE(entry.good());
    if (!entry.good()) {
      return default_value;
    }
    EXPECT_EQ(metahandle, entry.Get(META_HANDLE));
    return entry.Get(field);
  }
