gboolean BrowserWindowGtk::OnConfigure(GtkWidget* widget,
                                       GdkEventConfigure* event) {
  gfx::Rect bounds(event->x, event->y, event->width, event->height);

  if (bounds == configure_bounds_)
    return FALSE;

  GetLocationBar()->GetLocationEntry()->CloseOmniboxPopup();

  TabContents* tab = GetDisplayedTab();
  if (tab) {
    tab->web_contents()->GetRenderViewHost()->NotifyMoveOrResizeStarted();
  }

  if (bounds_.size() != bounds.size())
    UpdateWindowShape(bounds.width(), bounds.height());

  bounds_ = bounds;
  configure_bounds_ = bounds;

  window_configure_debounce_timer_.Stop();
  window_configure_debounce_timer_.Start(FROM_HERE,
      base::TimeDelta::FromMilliseconds(kDebounceTimeoutMilliseconds), this,
      &BrowserWindowGtk::OnDebouncedBoundsChanged);

  return FALSE;
}
