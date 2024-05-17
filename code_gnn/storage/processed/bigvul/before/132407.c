bool IsGestureEditingEnabled() {
  std::string keyboard_switch =
      base::CommandLine::ForCurrentProcess()->GetSwitchValueASCII(
          switches::kGestureEditing);
   return keyboard_switch != switches::kGestureEditingDisabled;
 }
