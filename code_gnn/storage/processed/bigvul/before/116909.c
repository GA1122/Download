void TabLoader::HandleTabClosedOrLoaded(NavigationController* tab) {
  RemoveTab(tab);
  if (loading_)
    LoadNextTab();
  if (tabs_loading_.empty() && tabs_to_load_.empty()) {
    base::TimeDelta time_to_load =
        base::TimeTicks::Now() - restore_started_;
    UMA_HISTOGRAM_CUSTOM_TIMES(
        "SessionRestore.AllTabsLoaded",
        time_to_load,
        base::TimeDelta::FromMilliseconds(10),
        base::TimeDelta::FromSeconds(100),
        100);
    std::string time_for_count =
        base::StringPrintf("SessionRestore.AllTabsLoaded_%d", tab_count_);
    base::Histogram* counter_for_count =
        base::Histogram::FactoryTimeGet(
            time_for_count,
            base::TimeDelta::FromMilliseconds(10),
            base::TimeDelta::FromSeconds(100),
            100,
            base::Histogram::kUmaTargetedHistogramFlag);
    counter_for_count->AddTime(time_to_load);
  }
}
