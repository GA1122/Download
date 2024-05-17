  static base::FilePath GetTestDataFilePath(const std::string& file_name) {
    base::FilePath source_dir;
    CHECK(base::PathService::Get(base::DIR_SOURCE_ROOT, &source_dir));
    auto test_data_dir = source_dir.AppendASCII("chrome")
                             .AppendASCII("test")
                             .AppendASCII("data")
                             .AppendASCII("chromeos")
                             .AppendASCII("file_manager");
    return test_data_dir.Append(base::FilePath::FromUTF8Unsafe(file_name));
  }
