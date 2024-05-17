void BrowserView::SetFocusToLocationBar(bool select_all) {
#if defined(OS_WIN)
  if (!force_location_bar_focus_ && !IsActive())
    return;
#endif

  LocationBarView* location_bar = GetLocationBarView();
  if (location_bar->IsLocationEntryFocusableInRootView()) {
    location_bar->FocusLocation(select_all);
  } else {
    views::FocusManager* focus_manager = GetFocusManager();
    DCHECK(focus_manager);
    focus_manager->ClearFocus();
  }
}
