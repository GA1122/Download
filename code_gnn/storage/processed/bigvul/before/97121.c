bool NavigationController::IsLikelyAutoNavigation(base::TimeTicks now) {
  return !user_gesture_observed_ &&
         (now - last_document_loaded_) < kMaxAutoNavigationTimeDelta;
}
