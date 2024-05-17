ShelfLayoutManager::~ShelfLayoutManager() {
  if (update_shelf_observer_)
    update_shelf_observer_->Detach();

  for (auto& observer : observers_)
    observer.WillDeleteShelfLayoutManager();
  display::Screen::GetScreen()->RemoveObserver(this);
  keyboard::KeyboardController::Get()->RemoveObserver(this);
  Shell::Get()->locale_update_controller()->RemoveObserver(this);
  Shell::Get()->RemoveShellObserver(this);
  Shell::Get()->lock_state_controller()->RemoveObserver(this);
  if (Shell::Get()->app_list_controller())
    Shell::Get()->app_list_controller()->RemoveObserver(this);
  if (Shell::Get()->overview_controller())
    Shell::Get()->overview_controller()->RemoveObserver(this);
}
