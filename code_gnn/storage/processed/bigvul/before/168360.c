void BrowserView::ShowAppMenu() {
  if (!toolbar_button_provider_->GetAppMenuButton())
    return;

  std::unique_ptr<ImmersiveRevealedLock> revealed_lock(
      immersive_mode_controller_->GetRevealedLock(
          ImmersiveModeController::ANIMATE_REVEAL_NO));

  toolbar_button_provider_->GetAppMenuButton()->Activate(nullptr);
}
