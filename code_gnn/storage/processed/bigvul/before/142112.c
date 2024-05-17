gfx::ImageSkia GetBookmarkManagedFolderIcon(SkColor text_color) {
#if defined(OS_WIN)
  return *ui::ResourceBundle::GetSharedInstance().GetImageSkiaNamed(
      IDR_BOOKMARK_BAR_FOLDER_MANAGED);
#else
  return GetFolderIcon(gfx::VectorIconId::FOLDER_MANAGED, text_color);
#endif
}
