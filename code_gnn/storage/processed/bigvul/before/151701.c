void Browser::NavigationStateChanged(WebContents* source,
                                     content::InvalidateTypes changed_flags) {
  tracked_objects::ScopedTracker tracking_profile1(
      FROM_HERE_WITH_EXPLICIT_FUNCTION(
          "466285 Browser::NavigationStateChanged::ScheduleUIUpdate"));
  if (changed_flags)
    ScheduleUIUpdate(source, changed_flags);

  tracked_objects::ScopedTracker tracking_profile2(
      FROM_HERE_WITH_EXPLICIT_FUNCTION(
          "466285 Browser::NavigationStateChanged::TabStateChanged"));
  if (changed_flags & (content::INVALIDATE_TYPE_URL |
                       content::INVALIDATE_TYPE_LOAD |
                       content::INVALIDATE_TYPE_TAB))
    command_controller_->TabStateChanged();

  if (hosted_app_controller_)
    hosted_app_controller_->UpdateLocationBarVisibility(true);
}
