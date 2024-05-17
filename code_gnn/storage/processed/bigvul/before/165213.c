void ArcVoiceInteractionFrameworkService::CaptureFullscreen(
    CaptureFullscreenCallback callback) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);

  if (!ValidateTimeSinceUserInteraction()) {
    std::move(callback).Run(std::vector<uint8_t>{});
    return;
  }

  if (chromeos::GetAshConfig() == ash::Config::MASH) {
    std::move(callback).Run(std::vector<uint8_t>{});
    return;
  }
  aura::Window* window = ash::Shell::GetPrimaryRootWindow();
  DCHECK(window);

  auto old_layer_owner = CreateLayerTreeForSnapshot(window);
  ui::GrabLayerSnapshotAsync(
      old_layer_owner->root(), gfx::Rect(window->bounds().size()),
      base::Bind(&EncodeAndReturnImage, base::Passed(std::move(callback)),
                 base::Passed(std::move(old_layer_owner))));
}
