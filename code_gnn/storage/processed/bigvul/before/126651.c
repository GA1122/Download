void TabStripModel::SetOpenerOfWebContentsAt(int index,
                                             WebContents* opener) {
  DCHECK(ContainsIndex(index));
  DCHECK(opener);
  contents_data_[index]->opener = opener;
}
