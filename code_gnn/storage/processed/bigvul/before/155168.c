void OmniboxViewViews::SetFocus() {
  std::unique_ptr<ImmersiveRevealedLock> focus_reveal_lock;
  if (location_bar_view_) {
    focus_reveal_lock.reset(
        BrowserView::GetBrowserViewForBrowser(location_bar_view_->browser())
            ->immersive_mode_controller()
            ->GetRevealedLock(ImmersiveModeController::ANIMATE_REVEAL_YES));
  }

  RequestFocus();
  model()->SetCaretVisibility(true);
  model()->ConsumeCtrlKey();
}
