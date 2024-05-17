  void ConfigureFlag(long flag, bool state) {
    (state ? set_mask : clear_mask) |= flag;
    DCHECK_EQ(0, set_mask & clear_mask) << flag << ":" << state;
  }
