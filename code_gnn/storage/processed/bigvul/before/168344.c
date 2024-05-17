void BrowserView::RevealTabStripIfNeeded() {
  if (!immersive_mode_controller_->IsEnabled())
    return;

  std::unique_ptr<ImmersiveRevealedLock> revealer(
      immersive_mode_controller_->GetRevealedLock(
          ImmersiveModeController::ANIMATE_REVEAL_YES));
  auto delete_revealer = base::BindOnce(
      [](std::unique_ptr<ImmersiveRevealedLock>) {}, std::move(revealer));
  constexpr auto kDefaultDelay = base::TimeDelta::FromSeconds(1);
  constexpr auto kZeroDelay = base::TimeDelta::FromSeconds(0);
  base::ThreadTaskRunnerHandle::Get()->PostDelayedTask(
      FROM_HERE, std::move(delete_revealer),
      g_disable_revealer_delay_for_testing ? kZeroDelay : kDefaultDelay);
}
