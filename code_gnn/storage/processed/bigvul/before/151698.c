void Browser::LostMouseLock() {
  exclusive_access_manager_->mouse_lock_controller()->LostMouseLock();
}
