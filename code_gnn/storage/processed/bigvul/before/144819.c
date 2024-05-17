  ExpectStateTransitionObserver(LifecycleUnit* lifecyle_unit,
                                LifecycleUnitState expected_state)
      : lifecycle_unit_(lifecyle_unit), expected_state_(expected_state) {
    lifecycle_unit_->AddObserver(this);
  }
