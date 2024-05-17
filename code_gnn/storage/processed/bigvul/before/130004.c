void ChromeBrowserMainParts::SetupMetricsAndFieldTrials() {
  TRACE_EVENT0("startup", "ChromeBrowserMainParts::SetupMetricsAndFieldTrials");

  metrics::MetricsService* metrics = browser_process_->metrics_service();
  field_trial_list_.reset(
      new base::FieldTrialList(metrics->CreateEntropyProvider().release()));

  const base::CommandLine* command_line =
      base::CommandLine::ForCurrentProcess();
  if (command_line->HasSwitch(switches::kEnableBenchmarking) ||
      command_line->HasSwitch(cc::switches::kEnableGpuBenchmarking)) {
    base::FieldTrial::EnableBenchmarking();
  }

  if (command_line->HasSwitch(switches::kForceFieldTrialParams)) {
    bool result = chrome_variations::AssociateParamsFromString(
        command_line->GetSwitchValueASCII(switches::kForceFieldTrialParams));
    CHECK(result) << "Invalid --" << switches::kForceFieldTrialParams
                  << " list specified.";
  }

  if (command_line->HasSwitch(switches::kForceFieldTrials)) {
    std::set<std::string> unforceable_field_trials;
#if defined(OFFICIAL_BUILD)
    unforceable_field_trials.insert("SettingsEnforcement");
#endif   

    bool result = base::FieldTrialList::CreateTrialsFromString(
        command_line->GetSwitchValueASCII(switches::kForceFieldTrials),
        base::FieldTrialList::DONT_ACTIVATE_TRIALS,
        unforceable_field_trials);
    CHECK(result) << "Invalid --" << switches::kForceFieldTrials
                  << " list specified.";
  }

#if defined(FIELDTRIAL_TESTING_ENABLED)
  if (!command_line->HasSwitch(switches::kDisableFieldTrialTestingConfig) &&
      !command_line->HasSwitch(switches::kForceFieldTrials) &&
      !command_line->HasSwitch(switches::kVariationsServerURL))
    chrome_variations::AssociateDefaultFieldTrialConfig();
#endif   

  if (command_line->HasSwitch(switches::kForceVariationIds)) {
    variations::VariationsHttpHeaderProvider* provider =
        variations::VariationsHttpHeaderProvider::GetInstance();
    bool result = provider->SetDefaultVariationIds(
        command_line->GetSwitchValueASCII(switches::kForceVariationIds));
    CHECK(result) << "Invalid --" << switches::kForceVariationIds
                  << " list specified.";
    metrics->AddSyntheticTrialObserver(provider);
  }
  chrome_variations::VariationsService* variations_service =
      browser_process_->variations_service();
  if (variations_service)
    variations_service->CreateTrialsFromSeed();

  browser_field_trials_.SetupFieldTrials();

  if (command_line->HasSwitch(switches::kEnableNavigationTracing) &&
      command_line->HasSwitch(switches::kTraceUploadURL)) {
    tracing::SetupNavigationTracing();
  }

  field_trial_synchronizer_ = new FieldTrialSynchronizer();

  metrics->InitializeMetricsRecordingState();

  const version_info::Channel channel = chrome::GetChannel();

  if (channel == version_info::Channel::UNKNOWN ||
      channel == version_info::Channel::CANARY ||
      channel == version_info::Channel::DEV) {
    media::AudioManager::EnableHangMonitor();
  }

  switch (channel) {
    case version_info::Channel::UNKNOWN:
    case version_info::Channel::CANARY:
      tracked_objects::ScopedTracker::Enable();
      break;

    case version_info::Channel::DEV:
    case version_info::Channel::BETA:
    case version_info::Channel::STABLE:
      break;
  }
}
