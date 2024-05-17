  AppCacheHostTest() {
    get_status_callback_ =
        base::Bind(&AppCacheHostTest::GetStatusCallback,
                   base::Unretained(this));
    start_update_callback_ =
        base::Bind(&AppCacheHostTest::StartUpdateCallback,
                   base::Unretained(this));
    swap_cache_callback_ =
        base::Bind(&AppCacheHostTest::SwapCacheCallback,
                   base::Unretained(this));
  }
