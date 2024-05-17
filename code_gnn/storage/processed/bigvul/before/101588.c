bool Browser::CanReloadContents(TabContents* source) const {
  return !is_devtools();
}
