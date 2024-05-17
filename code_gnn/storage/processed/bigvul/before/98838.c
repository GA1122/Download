  virtual Profile* GetOriginalProfile() {
    if (IsOffTheRecord())
      return linked_profile_;
    else
      return this;
  }
