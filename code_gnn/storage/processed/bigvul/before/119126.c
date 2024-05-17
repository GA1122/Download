  virtual ~TestFieldTrialObserver() {
    FieldTrialList::RemoveObserver(this);
  }
