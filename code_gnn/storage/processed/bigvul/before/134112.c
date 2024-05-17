uint32 IBusStateFromXState(unsigned int state) {
  return (state & (LockMask | ControlMask | ShiftMask | Mod1Mask |
                   Button1Mask | Button2Mask | Button3Mask));
}
