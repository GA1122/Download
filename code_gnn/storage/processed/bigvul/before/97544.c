  virtual void SetUp() {
    profile_.reset(new TestingProfile());
    base::FilePath program_path(FILE_PATH_LITERAL("chrome.exe"));
    command_line_.reset(new CommandLine(program_path));
  }
