bool InProcessBrowserTest::CreateUserDataDirectory() {
  CommandLine* command_line = CommandLine::ForCurrentProcess();
  base::FilePath user_data_dir =
      command_line->GetSwitchValuePath(switches::kUserDataDir);
  if (user_data_dir.empty()) {
    if (temp_user_data_dir_.CreateUniqueTempDir() &&
        temp_user_data_dir_.IsValid()) {
      user_data_dir = temp_user_data_dir_.path();
    } else {
      LOG(ERROR) << "Could not create temporary user data directory \""
                 << temp_user_data_dir_.path().value() << "\".";
      return false;
    }
  }
  return test_launcher_utils::OverrideUserDataDir(user_data_dir);
}
