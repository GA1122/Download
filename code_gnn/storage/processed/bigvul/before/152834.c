void MetricsWebContentsObserver::NavigationStopped() {
  NotifyPageEndAllLoads(END_STOP, UserInitiatedInfo::NotUserInitiated());
}
