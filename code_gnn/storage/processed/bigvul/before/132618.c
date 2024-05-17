std::vector<std::string> GetSideloadFontFiles() {
  std::vector<std::string> files;
  const base::CommandLine& command_line =
      *base::CommandLine::ForCurrentProcess();
  if (command_line.HasSwitch(switches::kRegisterFontFiles)) {
    base::SplitString(
        command_line.GetSwitchValueASCII(switches::kRegisterFontFiles),
        ';',
        &files);
  }
  return files;
}
