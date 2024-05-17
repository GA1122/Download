int GetModifiersFromACCEL(const ACCEL& accel) {
  int modifiers = EF_NONE;
  if (accel.fVirt & FSHIFT)
    modifiers |= EF_SHIFT_DOWN;
  if (accel.fVirt & FCONTROL)
    modifiers |= EF_CONTROL_DOWN;
  if (accel.fVirt & FALT)
    modifiers |= EF_ALT_DOWN;
  return modifiers;
}
