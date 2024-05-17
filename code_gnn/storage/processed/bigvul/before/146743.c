ScriptedAnimationController& Document::EnsureScriptedAnimationController() {
  if (!scripted_animation_controller_) {
    scripted_animation_controller_ = ScriptedAnimationController::Create(this);
    if (!GetPage())
      scripted_animation_controller_->Suspend();
  }
  return *scripted_animation_controller_;
}