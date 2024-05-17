VirtualKeyboardController::~VirtualKeyboardController() {
  Shell::GetInstance()->RemoveShellObserver(this);
  ui::DeviceDataManager::GetInstance()->RemoveObserver(this);
}
