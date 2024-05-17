LockContentsView::~LockContentsView() {
  Shell::Get()->accelerator_controller()->UnregisterAll(this);
  data_dispatcher_->RemoveObserver(this);
  Shell::Get()->login_screen_controller()->RemoveObserver(this);
  keyboard::KeyboardController::Get()->RemoveObserver(this);
  Shell::Get()->system_tray_notifier()->RemoveSystemTrayFocusObserver(this);

  if (unlock_attempt_ > 0) {
    Shell::Get()->metrics()->login_metrics_recorder()->RecordNumLoginAttempts(
        unlock_attempt_, false  );
  }
  chromeos::DBusThreadManager::Get()->GetPowerManagerClient()->RemoveObserver(
      this);
}
