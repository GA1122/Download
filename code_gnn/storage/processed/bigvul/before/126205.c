void Browser::NavigationStateChanged(const WebContents* source,
                                     unsigned changed_flags) {
  if (changed_flags)
    ScheduleUIUpdate(source, changed_flags);

  if (changed_flags & (content::INVALIDATE_TYPE_URL |
                       content::INVALIDATE_TYPE_LOAD))
    command_controller_->TabStateChanged();
}
