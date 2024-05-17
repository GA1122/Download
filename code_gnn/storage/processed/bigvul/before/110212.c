bool NaClProcessHost::LaunchSelLdr() {
  std::string channel_id = process_->GetHost()->CreateChannel();
  if (channel_id.empty())
    return false;

  CommandLine::StringType nacl_loader_prefix;
#if defined(OS_POSIX)
  nacl_loader_prefix = CommandLine::ForCurrentProcess()->GetSwitchValueNative(
      switches::kNaClLoaderCmdPrefix);
#endif   


#if defined(OS_MACOSX)
  int flags = ChildProcessHost::CHILD_NO_PIE;
#elif defined(OS_LINUX)
  int flags = nacl_loader_prefix.empty() ? ChildProcessHost::CHILD_ALLOW_SELF :
                                           ChildProcessHost::CHILD_NORMAL;
#else
  int flags = ChildProcessHost::CHILD_NORMAL;
#endif

  FilePath exe_path = ChildProcessHost::GetChildPath(flags);
  if (exe_path.empty())
    return false;

#if defined(OS_WIN)
  if (RunningOnWOW64()) {
    FilePath module_path;
    if (!PathService::Get(base::FILE_MODULE, &module_path))
      return false;
    exe_path = module_path.DirName().Append(chrome::kNaClAppName);
  }
#endif

  scoped_ptr<CommandLine> cmd_line(new CommandLine(exe_path));
  nacl::CopyNaClCommandLineArguments(cmd_line.get());

  cmd_line->AppendSwitchASCII(switches::kProcessType,
                              switches::kNaClLoaderProcess);
  cmd_line->AppendSwitchASCII(switches::kProcessChannelID, channel_id);
  if (logging::DialogsAreSuppressed())
    cmd_line->AppendSwitch(switches::kNoErrorDialogs);

  if (!nacl_loader_prefix.empty())
    cmd_line->PrependWrapper(nacl_loader_prefix);

  FilePath nacl_gdb = CommandLine::ForCurrentProcess()->GetSwitchValuePath(
      switches::kNaClGdb);
  if (!nacl_gdb.empty()) {
#if defined(OS_WIN)
    cmd_line->AppendSwitch(switches::kNoSandbox);
    scoped_ptr<CommandLine> gdb_cmd_line(
        GetCommandForLaunchWithGdb(nacl_gdb, cmd_line.get()));
    return base::LaunchProcess(*gdb_cmd_line, base::LaunchOptions(), NULL);
#elif defined(OS_LINUX)
    wait_for_nacl_gdb_ = true;
#endif
  }

#if defined(OS_WIN)
  if (RunningOnWOW64()) {
    return NaClBrokerService::GetInstance()->LaunchLoader(
        weak_factory_.GetWeakPtr(), channel_id);
  } else {
    process_->Launch(FilePath(), cmd_line.release());
  }
#elif defined(OS_POSIX)
  process_->Launch(nacl_loader_prefix.empty(),   
                   base::EnvironmentVector(),
                   cmd_line.release());
#endif

  return true;
}
