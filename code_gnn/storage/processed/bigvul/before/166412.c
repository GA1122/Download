  bool IsApplicable() {
    return !GetParam() ||
           !gl_.context()->GetVersionInfo()->is_desktop_core_profile;
  }
