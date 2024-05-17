  void RunLanguageDetectionTest(const base::FilePath::CharType* file_path) {
    base::FilePath test_path =
        GetTestFilePath("accessibility", "language-detection");
    {
      base::ScopedAllowBlockingForTesting allow_blocking;
      ASSERT_TRUE(base::PathExists(test_path)) << test_path.LossyDisplayName();
    }
    base::FilePath language_detection_file =
        test_path.Append(base::FilePath(file_path));

    RunTest(language_detection_file, "accessibility/language-detection");
  }
