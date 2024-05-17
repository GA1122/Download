void Browser::RequestToLockMouse(WebContents* web_contents,
                                 bool user_gesture,
                                 bool last_unlocked_by_target) {
  fullscreen_controller_->RequestToLockMouse(web_contents,
                                             user_gesture,
                                             last_unlocked_by_target);
}
