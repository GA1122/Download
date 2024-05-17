bool NaClProcessHost::LaunchNaClGdb(base::ProcessId pid) {
  CommandLine::StringType nacl_gdb =
      CommandLine::ForCurrentProcess()->GetSwitchValueNative(
          switches::kNaClGdb);
  CommandLine::StringVector argv;
  base::SplitString(nacl_gdb, static_cast<CommandLine::CharType>(' '), &argv);
  CommandLine cmd_line(argv);
  cmd_line.AppendArg("--eval-command");
  const FilePath::StringType& irt_path =
      NaClBrowser::GetInstance()->GetIrtFilePath().value();
  cmd_line.AppendArgNative(FILE_PATH_LITERAL("nacl-irt ") + irt_path);
  FilePath manifest_path = GetManifestPath();
  if (!manifest_path.empty()) {
    cmd_line.AppendArg("--eval-command");
    cmd_line.AppendArgNative(FILE_PATH_LITERAL("nacl-manifest ") +
                             manifest_path.value());
  }
  cmd_line.AppendArg("--eval-command");
  cmd_line.AppendArg("attach " + base::IntToString(pid));
  int fds[2];
  if (pipe(fds) != 0)
    return false;
  cmd_line.AppendArg("--eval-command");
  cmd_line.AppendArg("dump binary value /proc/" +
                     base::IntToString(base::GetCurrentProcId()) +
                     "/fd/" + base::IntToString(fds[1]) + " (char)0");
  FilePath script = CommandLine::ForCurrentProcess()->GetSwitchValuePath(
      switches::kNaClGdbScript);
  if (!script.empty()) {
    cmd_line.AppendArg("--command");
    cmd_line.AppendArgNative(script.value());
  }
  nacl_gdb_watcher_delegate_.reset(
      new NaClGdbWatchDelegate(
          fds[0], fds[1],
          base::Bind(&NaClProcessHost::OnNaClGdbAttached,
                     weak_factory_.GetWeakPtr())));
  MessageLoopForIO::current()->WatchFileDescriptor(
      fds[0],
      true,
      MessageLoopForIO::WATCH_READ,
      &nacl_gdb_watcher_,
      nacl_gdb_watcher_delegate_.get());
  return base::LaunchProcess(cmd_line, base::LaunchOptions(), NULL);
}
