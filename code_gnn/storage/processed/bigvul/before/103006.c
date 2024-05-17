void TabStripModel::TabNavigating(TabContentsWrapper* contents,
                                  PageTransition::Type transition) {
  if (ShouldForgetOpenersForTransition(transition)) {
    if (!IsNewTabAtEndOfTabStrip(contents)) {
      ForgetAllOpeners();
      ForgetGroup(contents);
    }
  }
}
