void RecordUMAStatistics(flags_ui::FlagsStorage* flags_storage) {
  std::set<std::string> switches;
  std::set<std::string> features;
  FlagsStateSingleton::GetFlagsState()->GetSwitchesAndFeaturesFromFlags(
      flags_storage, &switches, &features);
  ReportAboutFlagsHistogram("Launch.FlagsAtStartup", switches, features);
}
