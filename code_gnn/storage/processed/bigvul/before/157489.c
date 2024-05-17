  void OnRegistryKeyChanged() {
    DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
    if (--registry_watcher_count_ == 0) {
      UMA_HISTOGRAM_MEDIUM_TIMES(
          "DefaultBrowser.SettingsInteraction.RegistryWatcherDuration",
          base::TimeTicks::Now() - start_time_);

      ConcludeInteraction(ConcludeReason::REGISTRY_WATCHER);
    }
  }
