bool ShouldHighlightFields() {
  std::string group_name =
      base::FieldTrialList::FindFullName(kFillOnAccountSelectFieldTrialName);
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableFillOnAccountSelect) ||
      base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kEnableFillOnAccountSelect)) {
    return true;
  }

  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kEnableFillOnAccountSelectNoHighlighting)) {
    return false;
  }

  return group_name !=
         kFillOnAccountSelectFieldTrialEnabledWithNoHighlightGroup;
}
