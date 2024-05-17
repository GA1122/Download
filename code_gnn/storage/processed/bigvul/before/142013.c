  void AssertCBIsNull() {
    ASSERT_TRUE(cb.is_null());
    cb_already_run = true;
  }
