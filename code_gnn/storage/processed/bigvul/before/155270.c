  bool StartHandlerForClient(CrashReporterClient* client, int fd) {
    base::FilePath database_path;
    base::FilePath metrics_path;
    std::string url;
    std::map<std::string, std::string> process_annotations;
    std::vector<std::string> arguments;
    BuildHandlerArgs(client, &database_path, &metrics_path, &url,
                     &process_annotations, &arguments);

#if defined(OS_ANDROID)
    std::string browser_ptype;
    if (GetCrashReporterClient()->GetBrowserProcessType(&browser_ptype)) {
      process_annotations["ptype"] = browser_ptype;
    }
#endif   

    base::FilePath exe_dir;
    base::FilePath handler_path;
    if (!GetHandlerPath(&exe_dir, &handler_path)) {
      return false;
    }

#if defined(OS_ANDROID)
    if (use_java_handler_) {
      std::vector<std::string> env;
      if (!BuildEnvironmentWithApk(&env)) {
        return false;
      }

      bool result = GetCrashpadClient().StartJavaHandlerForClient(
          kCrashpadJavaMain, &env, database_path, metrics_path, url,
          process_annotations, arguments, fd);
      return result;
    }
#endif

    if (!SetLdLibraryPath(exe_dir)) {
      return false;
    }

    return GetCrashpadClient().StartHandlerForClient(
        handler_path, database_path, metrics_path, url, process_annotations,
        arguments, fd);
  }
