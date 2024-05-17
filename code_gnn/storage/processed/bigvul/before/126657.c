void TabStripModel::TabNavigating(TabContents* contents,
                                  content::PageTransition transition) {
  if (ShouldForgetOpenersForTransition(transition)) {
    if (!IsNewTabAtEndOfTabStrip(contents)) {
      ForgetAllOpeners();
      ForgetGroup(contents);
    }
  }
}
