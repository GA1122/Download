void ThreadWatcher::Initialize() {
  DCHECK(WatchDogThread::CurrentlyOnWatchDogThread());
  ThreadWatcherList::Register(this);

  const std::string response_time_histogram_name =
      "ThreadWatcher.ResponseTime." + thread_name_;
  response_time_histogram_ = base::Histogram::FactoryTimeGet(
      response_time_histogram_name,
      base::TimeDelta::FromMilliseconds(1),
      base::TimeDelta::FromSeconds(100), 50,
      base::Histogram::kUmaTargetedHistogramFlag);

  const std::string unresponsive_time_histogram_name =
      "ThreadWatcher.Unresponsive." + thread_name_;
  unresponsive_time_histogram_ = base::Histogram::FactoryTimeGet(
      unresponsive_time_histogram_name,
      base::TimeDelta::FromMilliseconds(1),
      base::TimeDelta::FromSeconds(100), 50,
      base::Histogram::kUmaTargetedHistogramFlag);

  const std::string responsive_count_histogram_name =
      "ThreadWatcher.ResponsiveThreads." + thread_name_;
  responsive_count_histogram_ = base::LinearHistogram::FactoryGet(
      responsive_count_histogram_name, 1, 10, 11,
      base::Histogram::kUmaTargetedHistogramFlag);

  const std::string unresponsive_count_histogram_name =
      "ThreadWatcher.UnresponsiveThreads." + thread_name_;
  unresponsive_count_histogram_ = base::LinearHistogram::FactoryGet(
      unresponsive_count_histogram_name, 1, 10, 11,
      base::Histogram::kUmaTargetedHistogramFlag);
}
