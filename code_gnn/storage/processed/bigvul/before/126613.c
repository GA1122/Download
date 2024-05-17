void TabStripModel::AppendTabContents(TabContents* contents,
                                      bool foreground) {
  int index = order_controller_->DetermineInsertionIndexForAppending();
  InsertTabContentsAt(index, contents,
                      foreground ? (ADD_INHERIT_GROUP | ADD_ACTIVE) :
                                   ADD_NONE);
}
