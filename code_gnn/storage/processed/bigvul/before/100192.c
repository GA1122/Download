  virtual bool ShouldExpire(
      const NavigationController::LoadCommittedDetails& details) const {
    return should_expire_;
  }
