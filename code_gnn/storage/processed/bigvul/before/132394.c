void RecordUMAStatistics(FlagsStorage* flags_storage) {
  std::set<std::string> flags = flags_storage->GetFlags();
  for (const std::string& flag : flags) {
    std::string action("AboutFlags_");
    action += flag;
    content::RecordComputedAction(action);
  }
  if (flags.size())
    content::RecordAction(base::UserMetricsAction("AboutFlags_StartupTick"));
  content::RecordAction(base::UserMetricsAction("StartupTick"));
}
