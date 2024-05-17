void GetFlagsExperimentsData(FlagsStorage* flags_storage,
                             FlagAccess access,
                             base::ListValue* supported_experiments,
                             base::ListValue* unsupported_experiments) {
  std::set<std::string> enabled_experiments;
  GetSanitizedEnabledFlags(flags_storage, &enabled_experiments);

  int current_platform = GetCurrentPlatform();

  for (size_t i = 0; i < num_experiments; ++i) {
    const Experiment& experiment = experiments[i];
    if (SkipConditionalExperiment(experiment, flags_storage))
      continue;

    base::DictionaryValue* data = new base::DictionaryValue();
    data->SetString("internal_name", experiment.internal_name);
    data->SetString("name",
                    l10n_util::GetStringUTF16(experiment.visible_name_id));
    data->SetString("description",
                    l10n_util::GetStringUTF16(
                        experiment.visible_description_id));

    base::ListValue* supported_platforms = new base::ListValue();
    AddOsStrings(experiment.supported_platforms, supported_platforms);
    data->Set("supported_platforms", supported_platforms);

    switch (experiment.type) {
      case Experiment::SINGLE_VALUE:
        data->SetBoolean(
            "enabled",
            enabled_experiments.count(experiment.internal_name) > 0);
        break;
      case Experiment::MULTI_VALUE:
      case Experiment::ENABLE_DISABLE_VALUE:
        data->Set("choices", CreateChoiceData(experiment, enabled_experiments));
        break;
      default:
        NOTREACHED();
    }

    bool supported = (experiment.supported_platforms & current_platform) != 0;
#if defined(OS_CHROMEOS)
    if (access == kOwnerAccessToFlags &&
        (experiment.supported_platforms & kOsCrOSOwnerOnly) != 0) {
      supported = true;
    }
#endif
    if (supported)
      supported_experiments->Append(data);
    else
      unsupported_experiments->Append(data);
  }
}
