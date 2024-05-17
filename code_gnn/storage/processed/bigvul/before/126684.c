static void InsertTabContentses(TabStripModel* tabstrip,
                                TabContents* contents1,
                                TabContents* contents2,
                                TabContents* contents3) {
  tabstrip->InsertTabContentsAt(GetInsertionIndex(tabstrip, contents1),
                                contents1, TabStripModel::ADD_INHERIT_GROUP);
  tabstrip->InsertTabContentsAt(GetInsertionIndex(tabstrip, contents2),
                                contents2, TabStripModel::ADD_INHERIT_GROUP);
  tabstrip->InsertTabContentsAt(GetInsertionIndex(tabstrip, contents3),
                                contents3, TabStripModel::ADD_INHERIT_GROUP);
}
