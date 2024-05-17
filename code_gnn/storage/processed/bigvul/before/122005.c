base::FilePath FindExeRelativeToSetupExe(const base::FilePath setup_exe_path,
                                         const wchar_t* exe_file) {
  if (!setup_exe_path.empty()) {
    base::FilePath exe_path(
        setup_exe_path.DirName().DirName().DirName().Append(exe_file));
    if (file_util::PathExists(exe_path))
      return exe_path;
    exe_path = setup_exe_path.DirName().DirName().Append(exe_file);
    if (file_util::PathExists(exe_path))
      return exe_path;
  }
  return base::FilePath();
}
