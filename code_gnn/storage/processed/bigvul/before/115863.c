  virtual void NavigationStateChanged(const TabContents* source,
                                      unsigned changed_flags) {
    navigation_state_change_count_++;
  }
