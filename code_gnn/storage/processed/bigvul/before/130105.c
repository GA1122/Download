MasterPreferences::MasterPreferences() : distribution_(NULL),
                                         preferences_read_from_file_(false),
                                         chrome_(true),
                                         multi_install_(false) {
  InitializeFromCommandLine(*base::CommandLine::ForCurrentProcess());
}
