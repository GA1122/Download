void FlagsState::SetExperimentEnabled(
    PrefService* prefs, const std::string& internal_name, bool enable) {
  needs_restart_ = true;

  size_t at_index = internal_name.find(about_flags::testing::kMultiSeparator);
  if (at_index != std::string::npos) {
    DCHECK(enable);
    DCHECK_NE(at_index, 0u);
    const std::string experiment_name = internal_name.substr(0, at_index);
    SetExperimentEnabled(prefs, experiment_name, false);

    if (internal_name != experiment_name + "@0") {
      std::set<std::string> enabled_experiments;
      GetSanitizedEnabledFlags(prefs, &enabled_experiments);
      enabled_experiments.insert(internal_name);
      SetEnabledFlags(prefs, enabled_experiments);
    }
    return;
  }

  std::set<std::string> enabled_experiments;
  GetSanitizedEnabledFlags(prefs, &enabled_experiments);

  const Experiment* e = NULL;
  for (size_t i = 0; i < num_experiments; ++i) {
    if (experiments[i].internal_name == internal_name) {
      e = experiments + i;
      break;
    }
  }
  DCHECK(e);

  if (e->type == Experiment::SINGLE_VALUE) {
    if (enable)
      enabled_experiments.insert(internal_name);
    else
      enabled_experiments.erase(internal_name);
  } else {
    if (enable) {
      enabled_experiments.insert(NameForChoice(*e, 0));
    } else {
      for (int i = 0; i < e->num_choices; ++i) {
        std::string choice_name = NameForChoice(*e, i);
        if (enabled_experiments.find(choice_name) !=
            enabled_experiments.end()) {
          enabled_experiments.erase(choice_name);
        }
      }
    }
  }

  SetEnabledFlags(prefs, enabled_experiments);
}
