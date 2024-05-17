  void ProfileCallback(BluetoothProfile* profile) {
    ++profile_callback_count_;
    last_profile_ = profile;
  }
