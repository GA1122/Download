  void ValidateBrowserAllocations(base::Value* dump_json) {
    SCOPED_TRACE("Validating Browser Allocations");
    base::Value* heaps_v2 =
        FindHeapsV2(base::Process::Current().Pid(), dump_json);

    if (GetParam() == switches::kMemlogModeAll ||
        GetParam() == switches::kMemlogModeBrowser ||
        GetParam() == switches::kMemlogModeMinimal) {
      ASSERT_TRUE(heaps_v2);
      ASSERT_NO_FATAL_FAILURE(
          ValidateDump(heaps_v2, kBrowserAllocSize * kBrowserAllocCount,
                       kBrowserAllocCount, "malloc", nullptr));
      ASSERT_NO_FATAL_FAILURE(
          ValidateDump(heaps_v2, total_variadic_allocations_,
                       kBrowserAllocCount, "malloc", nullptr));
      ASSERT_NO_FATAL_FAILURE(ValidateDump(
          heaps_v2, kPartitionAllocSize * kPartitionAllocCount,
          kPartitionAllocCount, "partition_alloc", kPartitionAllocTypeName));
    } else {
      ASSERT_FALSE(heaps_v2) << "There should be no heap dump for the browser.";
    }

    EXPECT_EQ(1, NumProcessesWithName(dump_json, "Browser"));
  }
