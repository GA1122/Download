  AutoplayPolicyTest() {
    embedded_test_server2()->AddDefaultHandlers(
        base::FilePath(FILE_PATH_LITERAL("chrome/test/data")));
    EXPECT_TRUE(embedded_test_server()->Start());
    EXPECT_TRUE(embedded_test_server2()->Start());
  }
