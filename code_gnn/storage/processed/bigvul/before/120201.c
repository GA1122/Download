void LayerTreeHost::AnimateLayers(base::TimeTicks time) {
  if (!settings_.accelerated_animation_enabled ||
      animation_registrar_->active_animation_controllers().empty())
    return;

  TRACE_EVENT0("cc", "LayerTreeHost::AnimateLayers");

  double monotonic_time = (time - base::TimeTicks()).InSecondsF();

  AnimationRegistrar::AnimationControllerMap copy =
      animation_registrar_->active_animation_controllers();
  for (AnimationRegistrar::AnimationControllerMap::iterator iter = copy.begin();
       iter != copy.end();
       ++iter) {
    (*iter).second->Animate(monotonic_time);
    bool start_ready_animations = true;
    (*iter).second->UpdateState(start_ready_animations, NULL);
  }
}
