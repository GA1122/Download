base::FilePath GetSharedMemoryPath(base::Environment* env) {
  std::string snap_name;
  if (env->GetVar("SNAP_NAME", &snap_name)) {
    return base::FilePath(std::string("/dev/shm/snap.") + snap_name + ".oxide");
  }

  std::string app_pkgname;
  if (env->GetVar("APP_ID", &app_pkgname)) {
    app_pkgname = app_pkgname.substr(0, app_pkgname.find("_"));
    if (!app_pkgname.empty()) {
      return base::FilePath(std::string("/dev/shm/") + app_pkgname + ".oxide");
    }
  }

  return base::FilePath("/dev/shm");
}
