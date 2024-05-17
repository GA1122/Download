SkBitmap Browser::GetCurrentPageIcon() const {
  TabContents* contents = GetSelectedTabContents();
  return contents ? contents->GetFavicon() : SkBitmap();
}
