bool MdFeedbackEnabled() {
  return base::CommandLine::ForCurrentProcess()->HasSwitch(
      ::switches::kEnableMaterialDesignFeedback);
}
