  virtual void Run() {
#if defined(OS_LINUX)
    std::string session_name = base::GetLinuxDistro();
#elif defined(OS_MACOSX)
    std::string session_name = SessionModelAssociator::GetHardwareModelName();
#elif defined(OS_WIN)
    std::string session_name = SessionModelAssociator::GetComputerName();
#else
    std::string session_name;
#endif
    if (session_name == "Unknown" || session_name.empty()) {
      session_name = base::SysInfo::OperatingSystemName();
    }
    associator_.Call(FROM_HERE,
                     &SessionModelAssociator::OnSessionNameInitialized,
                     session_name);
  }
