void Browser::RequestToLockMouse(WebContents* web_contents,
                                 bool user_gesture,
                                 bool last_unlocked_by_target) {
  exclusive_access_manager_->mouse_lock_controller()->RequestToLockMouse(
      web_contents, user_gesture, last_unlocked_by_target);
}
