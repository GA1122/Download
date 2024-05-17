std::string ContextualSearchFieldTrial::GetSwitch(const std::string& name) {
  if (!HasSwitch(name))
    return std::string();
  else
    return base::CommandLine::ForCurrentProcess()->GetSwitchValueASCII(name);
}
