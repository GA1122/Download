ShelfLayoutManager::ShelfLayoutManager(ShelfWidget* shelf_widget, Shelf* shelf)
    : shelf_widget_(shelf_widget),
      shelf_(shelf),
      is_background_blur_enabled_(
          app_list_features::IsBackgroundBlurEnabled()) {
  DCHECK(shelf_widget_);
  DCHECK(shelf_);
  Shell::Get()->AddShellObserver(this);
  Shell::Get()->overview_controller()->AddObserver(this);
  Shell::Get()->app_list_controller()->AddObserver(this);
  Shell::Get()->lock_state_controller()->AddObserver(this);
  Shell::Get()->activation_client()->AddObserver(this);
  Shell::Get()->locale_update_controller()->AddObserver(this);
  state_.session_state = Shell::Get()->session_controller()->GetSessionState();
  keyboard::KeyboardController::Get()->AddObserver(this);
  wallpaper_controller_observer_.Add(Shell::Get()->wallpaper_controller());
  display::Screen::GetScreen()->AddObserver(this);
}
