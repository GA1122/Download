guint32 IBusStateFromXFlags(unsigned int flags) {
  return (flags & (LockMask | ControlMask | ShiftMask | Mod1Mask |
                   Button1Mask | Button2Mask | Button3Mask));
}
