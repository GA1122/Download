  void RunRegressionTest(const base::FilePath::CharType* file_path) {
    base::FilePath test_path = GetTestFilePath("accessibility", "regression");
    base::FilePath test_file = test_path.Append(base::FilePath(file_path));

    RunTest(test_file, "accessibility/regression");
  }
