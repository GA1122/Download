int GetModifiersFromKeyState() {
  int modifiers = EF_NONE;
  if (base::win::IsShiftPressed())
    modifiers |= EF_SHIFT_DOWN;
  if (base::win::IsCtrlPressed())
    modifiers |= EF_CONTROL_DOWN;
  if (base::win::IsAltPressed())
    modifiers |= EF_ALT_DOWN;
  if (base::win::IsAltGrPressed())
    modifiers |= EF_ALTGR_DOWN;
  return modifiers;
}
