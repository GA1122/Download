void LoadingPredictor::CleanupAbandonedHintsAndNavigations(
    const NavigationID& navigation_id) {
  base::TimeTicks time_now = base::TimeTicks::Now();
  const base::TimeDelta max_navigation_age =
      base::TimeDelta::FromSeconds(config_.max_navigation_lifetime_seconds);

  for (auto it = active_hints_.begin(); it != active_hints_.end();) {
    base::TimeDelta prefetch_age = time_now - it->second;
    if (prefetch_age > max_navigation_age) {
      it = CancelActiveHint(it);
    } else {
      ++it;
    }
  }

  for (auto it = active_navigations_.begin();
       it != active_navigations_.end();) {
    if ((it->tab_id == navigation_id.tab_id) ||
        (time_now - it->creation_time > max_navigation_age)) {
      CancelActiveHint(active_hints_.find(it->main_frame_url));
      it = active_navigations_.erase(it);
    } else {
      ++it;
    }
  }
}