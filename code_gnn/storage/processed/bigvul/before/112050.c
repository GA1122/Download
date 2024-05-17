  ValueType GetField(int64 metahandle, FieldType field,
      ValueType default_value) {
    ReadTransaction trans(FROM_HERE, directory());
    Entry entry(&trans, GET_BY_HANDLE, metahandle);
    EXPECT_TRUE(entry.good());
    if (!entry.good()) {
      return default_value;
    }
    EXPECT_EQ(metahandle, entry.Get(META_HANDLE));
    return entry.Get(field);
  }
