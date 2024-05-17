base::FilePath GetSubprocessPath(base::Environment* env) {
  std::string override_subprocess_path =
      GetEnvironmentOption("SUBPROCESS_PATH", env);
  if (!override_subprocess_path.empty()) {
#if defined(OS_POSIX)
    base::FilePath subprocess_path(override_subprocess_path);
#else
    base::FilePath subprocess_path(base::UTF8ToUTF16(override_subprocess_path));
#endif
    return base::MakeAbsoluteFilePath(subprocess_path);
  }

  base::FilePath subprocess_exe =
      base::FilePath(FILE_PATH_LITERAL(OXIDE_SUBPROCESS_PATH));
  if (subprocess_exe.IsAbsolute()) {
    return subprocess_exe;
  }

#if defined(OS_LINUX)
  Dl_info info;
  int rv = dladdr(reinterpret_cast<void *>(BrowserProcessMain::GetInstance),
                  &info);
  DCHECK_NE(rv, 0) << "Failed to determine module path";

  base::FilePath subprocess_rel(subprocess_exe);
  subprocess_exe = base::FilePath(info.dli_fname).DirName();

  std::vector<base::FilePath::StringType> components;
  subprocess_rel.GetComponents(&components);
  for (size_t i = 0; i < components.size(); ++i) {
    subprocess_exe = subprocess_exe.Append(components[i]);
  }
#else
# error "GetSubprocessPath is not implemented for this platform"
#endif

  return subprocess_exe;
}
