void UserSelectionScreen::Init(const user_manager::UserList& users) {
  users_ = users;

  ui::UserActivityDetector* activity_detector = ui::UserActivityDetector::Get();
  if (activity_detector && !activity_detector->HasObserver(this))
    activity_detector->AddObserver(this);
 }
