static int GetInsertionIndex(TabStripModel* tabstrip,
                             TabContents* contents) {
  return tabstrip->order_controller()->DetermineInsertionIndex(
      contents, content::PAGE_TRANSITION_LINK, false);
}
