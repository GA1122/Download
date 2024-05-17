void AutomationInternalCustomBindings::UpdateOverallTreeChangeObserverFilter() {
  tree_change_observer_overall_filter_ =
      api::automation::TREE_CHANGE_OBSERVER_FILTER_NOTREECHANGES;
  for (const auto& observer : tree_change_observers_) {
    tree_change_observer_overall_filter_ =
        std::max(observer.filter, tree_change_observer_overall_filter_);
  }
}
