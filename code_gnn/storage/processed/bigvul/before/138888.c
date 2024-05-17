void WallpaperManager::InitializeWallpaper() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  user_manager::UserManager* user_manager = user_manager::UserManager::Get();

  if (ShouldUseCustomizedDefaultWallpaper()) {
    SetDefaultWallpaperPath(GetCustomizedWallpaperDefaultRescaledFileName(
                                wallpaper::kSmallWallpaperSuffix),
                            std::unique_ptr<gfx::ImageSkia>(),
                            GetCustomizedWallpaperDefaultRescaledFileName(
                                wallpaper::kLargeWallpaperSuffix),
                            std::unique_ptr<gfx::ImageSkia>());
  }

  base::CommandLine* command_line = GetCommandLine();
  if (command_line->HasSwitch(chromeos::switches::kGuestSession)) {
    return;
  }

  if (command_line->HasSwitch(::switches::kTestType))
    WizardController::SetZeroDelays();

  if (WizardController::IsZeroDelayEnabled()) {
    ash::Shell::Get()->wallpaper_controller()->CreateEmptyWallpaper();
    return;
  }

  if (!user_manager->IsUserLoggedIn()) {
    if (!StartupUtils::IsDeviceRegistered())
      SetDefaultWallpaperDelayed(user_manager::SignInAccountId());
    else
      InitializeRegisteredDeviceWallpaper();
    return;
  }
  SetUserWallpaperDelayed(user_manager->GetActiveUser()->GetAccountId());
}
