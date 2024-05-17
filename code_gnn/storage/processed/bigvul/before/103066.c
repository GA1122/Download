  ~MockTabStripModelObserver() {
    STLDeleteContainerPointers(states_.begin(), states_.end());
  }
