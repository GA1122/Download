bool NaClProcessHost::LaunchSelLdr() {
  if (!CreateChannel())
    return false;

  CommandLine::StringType nacl_loader_prefix;
#if defined(OS_POSIX)
  nacl_loader_prefix = CommandLine::ForCurrentProcess()->GetSwitchValueNative(
      switches::kNaClLoaderCmdPrefix);
#endif   

  FilePath exe_path = GetChildPath(nacl_loader_prefix.empty());
  if (exe_path.empty())
    return false;

  CommandLine* cmd_line = new CommandLine(exe_path);
  nacl::CopyNaClCommandLineArguments(cmd_line);

  cmd_line->AppendSwitchASCII(switches::kProcessType,
                              switches::kNaClLoaderProcess);

  cmd_line->AppendSwitchASCII(switches::kProcessChannelID, channel_id());

  if (!nacl_loader_prefix.empty())
    cmd_line->PrependWrapper(nacl_loader_prefix);

#if defined(OS_WIN)
  if (running_on_wow64_) {
    return NaClBrokerService::GetInstance()->LaunchLoader(
        this, ASCIIToWide(channel_id()));
  } else {
    BrowserChildProcessHost::Launch(FilePath(), cmd_line);
  }
#elif defined(OS_POSIX)
  BrowserChildProcessHost::Launch(nacl_loader_prefix.empty(),   
                                  base::environment_vector(),
                                  cmd_line);
#endif

  return true;
}
