scoped_ptr<CommandLine> NaClProcessHost::GetCommandForLaunchWithGdb(
    const FilePath& nacl_gdb,
    CommandLine* line) {
  CommandLine* cmd_line = new CommandLine(nacl_gdb);
  cmd_line->AppendArg("--eval-command");
  const FilePath::StringType& irt_path =
      NaClBrowser::GetInstance()->GetIrtFilePath().value();
  cmd_line->AppendArgNative(FILE_PATH_LITERAL("nacl-irt ") + irt_path);
  FilePath manifest_path = GetManifestPath();
  if (!manifest_path.empty()) {
    cmd_line->AppendArg("--eval-command");
    cmd_line->AppendArgNative(FILE_PATH_LITERAL("nacl-manifest ") +
                              manifest_path.value());
  }
  FilePath script = CommandLine::ForCurrentProcess()->GetSwitchValuePath(
      switches::kNaClGdbScript);
  if (!script.empty()) {
    cmd_line->AppendArg("--command");
    cmd_line->AppendArgNative(script.value());
  }
  cmd_line->AppendArg("--args");
  const CommandLine::StringVector& argv = line->argv();
  for (size_t i = 0; i < argv.size(); i++) {
    cmd_line->AppendArgNative(argv[i]);
  }
  return scoped_ptr<CommandLine>(cmd_line);
}
