bool IsAppHostPresent() {
  base::FilePath app_host_exe = GetAnyAppHostPath();
  return !app_host_exe.empty();
}
