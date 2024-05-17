void WallpaperManager::OnWindowDestroying(aura::Window* window) {
  window_observer_.Remove(window);
  chromeos::WallpaperWindowStateManager::RestoreWindows(
      user_manager::UserManager::Get()->GetActiveUser()->username_hash());
}
