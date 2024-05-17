void VRDisplay::ContextDestroyed(ExecutionContext*) {
  ForceExitPresent();
  scripted_animation_controller_.Clear();
}
