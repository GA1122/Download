LifecycleUnit::SortKey TabLifecycleUnitSource::TabLifecycleUnit::GetSortKey()
    const {
  if (base::FeatureList::IsEnabled(features::kTabRanker)) {
    base::Optional<float> reactivation_score =
        resource_coordinator::TabActivityWatcher::GetInstance()
            ->CalculateReactivationScore(GetWebContents());
    if (reactivation_score.has_value())
      return SortKey(reactivation_score.value(), last_focused_time_);
    return SortKey(SortKey::kMaxScore, last_focused_time_);
  }

  return SortKey(last_focused_time_);
}
