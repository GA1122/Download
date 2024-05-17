void GamepadProvider::RegisterForUserGesture(const base::Closure& closure) {
  base::AutoLock lock(user_gesture_lock_);
  user_gesture_observers_.push_back(
      ClosureAndThread(closure, base::ThreadTaskRunnerHandle::Get()));
}
