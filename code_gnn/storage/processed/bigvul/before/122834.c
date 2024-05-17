  void OnUpdateVSyncParameters(
      base::TimeTicks timebase, base::TimeDelta interval) {
    DCHECK(CalledOnValidThread());
    DCHECK(client_);
    double monotonicTimebase = timebase.ToInternalValue() /
      static_cast<double>(base::Time::kMicrosecondsPerSecond);
    double intervalInSeconds = interval.ToInternalValue() /
      static_cast<double>(base::Time::kMicrosecondsPerSecond);
    client_->onVSyncParametersChanged(monotonicTimebase, intervalInSeconds);
  }
