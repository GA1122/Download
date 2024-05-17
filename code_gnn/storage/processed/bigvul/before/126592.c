void TabStripGtk::TabReplacedAt(TabStripModel* tab_strip_model,
                                TabContents* old_contents,
                                TabContents* new_contents,
                                int index) {
  TabChangedAt(new_contents, index, ALL);
}
