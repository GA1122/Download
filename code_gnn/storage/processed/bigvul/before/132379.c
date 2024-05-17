  void SetEventBlocker(
      scoped_ptr<ScopedDisableInternalMouseAndKeyboard> blocker) {
    Shell::GetInstance()->maximize_mode_controller()->event_blocker_ =
        blocker.Pass();
   }
