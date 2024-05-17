  void RunAomTest(const base::FilePath::CharType* file_path) {
    base::FilePath test_path = GetTestFilePath("accessibility", "aom");
    {
      base::ScopedAllowBlockingForTesting allow_blocking;
      ASSERT_TRUE(base::PathExists(test_path)) << test_path.LossyDisplayName();
    }
    base::FilePath aom_file = test_path.Append(base::FilePath(file_path));

    RunTest(aom_file, "accessibility/aom");
  }
