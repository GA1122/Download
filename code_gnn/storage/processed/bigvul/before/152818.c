void MetricsWebContentsObserver::AddTestingObserver(TestingObserver* observer) {
  if (!testing_observers_.HasObserver(observer))
    testing_observers_.AddObserver(observer);
}
