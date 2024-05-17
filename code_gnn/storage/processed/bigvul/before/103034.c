static int GetInsertionIndex(TabStripModel* tabstrip,
                             TabContentsWrapper* contents) {
  return tabstrip->order_controller()->DetermineInsertionIndex(
      contents, PageTransition::LINK, false);
}
