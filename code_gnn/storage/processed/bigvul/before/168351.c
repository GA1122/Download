void BrowserView::SetFocusToLocationBar(bool select_all) {
#if defined(OS_WIN) || defined(OS_CHROMEOS)
  if (!force_location_bar_focus_ && !IsActive())
    return;
#endif

  std::unique_ptr<ImmersiveRevealedLock> focus_reveal_lock(
      immersive_mode_controller_->GetRevealedLock(
          ImmersiveModeController::ANIMATE_REVEAL_YES));

  LocationBarView* location_bar = GetLocationBarView();
  if (location_bar->omnibox_view()->IsFocusable()) {
    location_bar->FocusLocation(select_all);
  } else {
    views::FocusManager* focus_manager = GetFocusManager();
    DCHECK(focus_manager);
    focus_manager->ClearFocus();
  }
}
