SkBitmap Browser::GetCurrentPageIcon() const {
  TabContentsWrapper* contents = GetSelectedTabContentsWrapper();
  return contents ? contents->favicon_tab_helper()->GetFavicon() : SkBitmap();
}
