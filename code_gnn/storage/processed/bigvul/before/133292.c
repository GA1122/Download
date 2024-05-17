void HandleRotateActiveWindow() {
  base::RecordAction(UserMetricsAction("Accel_Rotate_Window"));
  aura::Window* active_window = wm::GetActiveWindow();
  if (active_window) {
    active_window->layer()->GetAnimator()->set_preemption_strategy(
        ui::LayerAnimator::REPLACE_QUEUED_ANIMATIONS);
    active_window->layer()->GetAnimator()->StartAnimation(
        new ui::LayerAnimationSequence(
            new WindowRotation(360, active_window->layer())));
  }
}
