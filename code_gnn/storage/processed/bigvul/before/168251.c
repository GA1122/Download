void BrowserView::FocusToolbar() {
  std::unique_ptr<ImmersiveRevealedLock> focus_reveal_lock(
      immersive_mode_controller_->GetRevealedLock(
          ImmersiveModeController::ANIMATE_REVEAL_YES));

  toolbar_button_provider_->FocusToolbar();
}
