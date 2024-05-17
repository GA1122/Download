  void LoadFile(const base::FilePath& file) {
    base::FilePath webui(FILE_PATH_LITERAL("webui"));
    RunTest(ui_test_utils::GetTestUrl(webui, file));
  }
