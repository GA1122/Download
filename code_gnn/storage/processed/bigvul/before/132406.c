bool IsFloatingVirtualKeyboardEnabled() {
  std::string floating_virtual_keyboard_switch =
      base::CommandLine::ForCurrentProcess()->GetSwitchValueASCII(
          switches::kFloatingVirtualKeyboard);
  return floating_virtual_keyboard_switch ==
      switches::kFloatingVirtualKeyboardEnabled;
}
