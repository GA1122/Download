bool TabStripModel::IsAppTab(int index) const {
  WebContents* contents = GetWebContentsAt(index);
  return contents && extensions::TabHelper::FromWebContents(contents)->is_app();
}
