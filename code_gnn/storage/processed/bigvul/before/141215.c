void Document::ServiceScriptedAnimations(
    base::TimeTicks monotonic_animation_start_time) {
  if (!scripted_animation_controller_)
    return;
  scripted_animation_controller_->ServiceScriptedAnimations(
      monotonic_animation_start_time);
}
