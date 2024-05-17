void MetricsWebContentsObserver::WebContentsDestroyed() {
  NotifyPageEndAllLoads(END_CLOSE, UserInitiatedInfo::NotUserInitiated());

  committed_load_ = nullptr;
  provisional_loads_.clear();
  aborted_provisional_loads_.clear();

  for (auto& observer : testing_observers_)
    observer.OnGoingAway();
}
