void Browser::NavigationStateChanged(WebContents* source,
                                     content::InvalidateTypes changed_flags) {
  if (changed_flags)
    ScheduleUIUpdate(source, changed_flags);

  if (changed_flags & (content::INVALIDATE_TYPE_URL |
                       content::INVALIDATE_TYPE_LOAD |
                       content::INVALIDATE_TYPE_TAB))
    command_controller_->TabStateChanged();

  if (hosted_app_controller_)
    hosted_app_controller_->UpdateLocationBarVisibility(true);
}
