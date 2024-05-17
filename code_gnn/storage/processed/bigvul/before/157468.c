  void ConcludeInteraction(ConcludeReason conclude_reason) {
    DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

    UMA_HISTOGRAM_ENUMERATION(
        "DefaultBrowser.SettingsInteraction.ConcludeReason", conclude_reason,
        NUM_CONCLUDE_REASON_TYPES);
    on_finished_callback_.Run();
    delete instance_;
    instance_ = nullptr;
  }
