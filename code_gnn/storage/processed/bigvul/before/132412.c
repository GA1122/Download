 bool IsMaterialDesignEnabled() {
   return !base::CommandLine::ForCurrentProcess()->HasSwitch(
       switches::kDisableNewMDInputView);
}
