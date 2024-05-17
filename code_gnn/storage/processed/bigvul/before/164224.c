  void Verify_Reinitialized(ReinitTestCase test_case) {
    EXPECT_TRUE(observer_->observed_old_storage_.get());
    EXPECT_TRUE(observer_->observed_old_storage_->storage() != storage());
    EXPECT_FALSE(PathExists(
        temp_directory_.GetPath().AppendASCII("Cache").AppendASCII("index")));
    EXPECT_FALSE(PathExists(temp_directory_.GetPath().AppendASCII("Index")));

    if (test_case == CORRUPT_SQL_ON_INSTALL) {
      AppCacheStorageImpl* storage = static_cast<AppCacheStorageImpl*>(
          observer_->observed_old_storage_->storage());
      EXPECT_TRUE(storage->database_->was_corruption_detected());
    }

    if (test_case == CORRUPT_CACHE_ON_INSTALL ||
        test_case == CORRUPT_SQL_ON_INSTALL) {
      EXPECT_TRUE(frontend_.error_event_was_raised_);
      AppCacheHost* host1 = backend_->GetHost(1);
      EXPECT_FALSE(host1->associated_cache());
      EXPECT_FALSE(host1->group_being_updated_.get());
      EXPECT_TRUE(host1->disabled_storage_reference_.get());
    } else {
      ASSERT_EQ(CORRUPT_CACHE_ON_LOAD_EXISTING, test_case);
      AppCacheHost* host2 = backend_->GetHost(2);
      EXPECT_TRUE(host2->disabled_storage_reference_.get());
    }

    service_->RemoveObserver(observer_.get());
    handler_.reset();
    backend_.reset();
    observer_.reset();
    TestFinished();
  }
