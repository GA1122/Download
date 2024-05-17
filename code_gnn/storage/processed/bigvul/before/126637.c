void TabStripModel::InsertWebContentsAt(int index,
                                        WebContents* contents,
                                        int add_types) {
  TabContents* tab_contents = TabContents::FromWebContents(contents);
  DCHECK(tab_contents);
  InsertTabContentsAt(index, tab_contents, add_types);
}
