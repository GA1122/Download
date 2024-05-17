bool Browser::IsMouseLocked() const {
  return exclusive_access_manager_->mouse_lock_controller()->IsMouseLocked();
}
