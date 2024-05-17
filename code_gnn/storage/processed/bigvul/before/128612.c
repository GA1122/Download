void RecordUMAStatistics(flags_ui::FlagsStorage* flags_storage) {
  const std::set<std::string> switches =
      FlagsStateSingleton::GetFlagsState()->GetSwitchesFromFlags(flags_storage);
  const std::set<std::string> features =
      FlagsStateSingleton::GetFlagsState()->GetFeaturesFromFlags(flags_storage);
  ReportAboutFlagsHistogram("Launch.FlagsAtStartup", switches, features);
}
