UserSelectionScreen::~UserSelectionScreen() {
  proximity_auth::ScreenlockBridge::Get()->SetLockHandler(nullptr);
  ui::UserActivityDetector* activity_detector = ui::UserActivityDetector::Get();
  if (activity_detector && activity_detector->HasObserver(this))
    activity_detector->RemoveObserver(this);
}
