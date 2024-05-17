void LoginDisplayHostWebUI::OnFinalize() {
  DVLOG(1) << "Finalizing LoginDisplayHost. User session starting";

  switch (finalize_animation_type_) {
    case ANIMATION_NONE:
      ShutdownDisplayHost();
      break;
    case ANIMATION_WORKSPACE:
      if (ash::Shell::HasInstance())
        ScheduleWorkspaceAnimation();

      ShutdownDisplayHost();
      break;
    case ANIMATION_FADE_OUT:
      ScheduleFadeOutAnimation(kDefaultFadeTimeMs);
      break;
    case ANIMATION_ADD_USER:
      break;
    default:
      break;
  }
}
