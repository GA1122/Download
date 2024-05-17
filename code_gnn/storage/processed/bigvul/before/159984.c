void DiskCacheBackendTest::BackendRecoverWithEviction() {
  success_ = false;
  ASSERT_TRUE(CopyTestCache("insert_load1"));
  DisableFirstCleanup();

  SetMask(0xf);
  SetMaxSize(0x1000);

  InitCache();
  DisableIntegrityCheck();
}
