  base::FilePath Initialize(bool dump_at_crash) {
    base::FilePath database_path;
    base::FilePath metrics_path;
    std::string url;
    std::map<std::string, std::string> process_annotations;
    std::vector<std::string> arguments;
    BuildHandlerArgs(GetCrashReporterClient(), &database_path, &metrics_path,
                     &url, &process_annotations, &arguments);

    base::FilePath exe_dir;
    base::FilePath handler_path;
    if (!GetHandlerPath(&exe_dir, &handler_path)) {
      return database_path;
    }

    if (crashpad::SetSanitizationInfo(GetCrashReporterClient(),
                                      &browser_sanitization_info_)) {
      arguments.push_back(base::StringPrintf("--sanitization-information=%p",
                                             &browser_sanitization_info_));
    }

#if defined(OS_ANDROID)
    if (!base::PathExists(handler_path)) {
      use_java_handler_ = true;
    }

    if (!dump_at_crash) {
      return database_path;
    }

    if (use_java_handler_) {
      std::vector<std::string> env;
      if (!BuildEnvironmentWithApk(&env)) {
        return database_path;
      }

      bool result = GetCrashpadClient().StartJavaHandlerAtCrash(
          kCrashpadJavaMain, &env, database_path, metrics_path, url,
          process_annotations, arguments);
      DCHECK(result);
      return database_path;
    }
#endif

    if (!SetLdLibraryPath(exe_dir)) {
      return database_path;
    }

    bool result = GetCrashpadClient().StartHandlerAtCrash(
        handler_path, database_path, metrics_path, url, process_annotations,
        arguments);
    DCHECK(result);
    return database_path;
  }
