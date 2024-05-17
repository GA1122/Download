Document::EnsureIntersectionObserverController() {
  if (!intersection_observer_controller_)
    intersection_observer_controller_ =
        IntersectionObserverController::Create(this);
  return *intersection_observer_controller_;
}
