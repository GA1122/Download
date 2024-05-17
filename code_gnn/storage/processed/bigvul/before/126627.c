int TabStripModel::GetIndexOfTabContents(const TabContents* contents) const {
  return GetIndexOfWebContents(contents->web_contents());
}
