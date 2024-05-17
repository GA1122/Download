void DiskCacheBackendTest::BackendRecoverInsert() {
  BackendTransaction("insert_empty1", 0, false);
  ASSERT_TRUE(success_) << "insert_empty1";
  BackendTransaction("insert_empty2", 0, false);
  ASSERT_TRUE(success_) << "insert_empty2";
  BackendTransaction("insert_empty3", 0, false);
  ASSERT_TRUE(success_) << "insert_empty3";

  BackendTransaction("insert_one1", 1, false);
  ASSERT_TRUE(success_) << "insert_one1";
  BackendTransaction("insert_one2", 1, false);
  ASSERT_TRUE(success_) << "insert_one2";
  BackendTransaction("insert_one3", 1, false);
  ASSERT_TRUE(success_) << "insert_one3";

  BackendTransaction("insert_load1", 100, true);
  ASSERT_TRUE(success_) << "insert_load1";
  BackendTransaction("insert_load2", 100, true);
  ASSERT_TRUE(success_) << "insert_load2";
}
