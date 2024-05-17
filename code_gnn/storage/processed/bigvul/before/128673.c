bool ContextualSearchFieldTrial::HasSwitch(const std::string& name) {
  return base::CommandLine::ForCurrentProcess()->HasSwitch(name);
}
