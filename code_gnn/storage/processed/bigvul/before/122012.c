base::FilePath GetDevelopmentExe(const wchar_t* exe_file) {
  base::FilePath current_directory;
  if (PathService::Get(base::DIR_EXE, &current_directory)) {
    base::FilePath chrome_exe_path(current_directory.Append(exe_file));
    if (file_util::PathExists(chrome_exe_path))
      return chrome_exe_path;
  }
  return base::FilePath();
}
