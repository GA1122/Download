  std::string GetPinnedState(const TabStripModel& model) {
    std::string actual;
    for (int i = 0; i < model.count(); ++i) {
      if (i > 0)
        actual += " ";

      actual +=
          base::IntToString(GetID(model.GetTabContentsAt(i)->tab_contents()));

      if (model.IsAppTab(i))
        actual += "a";

      if (model.IsTabPinned(i))
        actual += "p";
    }
    return actual;
  }
