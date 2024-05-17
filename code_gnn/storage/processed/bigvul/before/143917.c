 void InitLoggingFromCommandLine(const base::CommandLine& command_line) {
   base::FilePath log_filename;
   std::string filename = command_line.GetSwitchValueASCII(switches::kLogFile);
  if (filename.empty()) {
    base::PathService::Get(base::DIR_EXE, &log_filename);
    log_filename = log_filename.AppendASCII("webrunner.log");
  } else {
    log_filename = base::FilePath::FromUTF8Unsafe(filename);
  }

  logging::LoggingSettings settings;
  settings.logging_dest = logging::LOG_TO_ALL;
  settings.log_file = log_filename.value().c_str();
  settings.delete_old = logging::DELETE_OLD_LOG_FILE;
  logging::InitLogging(settings);
  logging::SetLogItems(true  , true  ,
                       true  , false  );
}
