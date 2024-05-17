gfx::ImageSkia GetBookmarkSupervisedFolderIcon(SkColor text_color) {
#if defined(OS_WIN)
  return *ui::ResourceBundle::GetSharedInstance().GetImageSkiaNamed(
      IDR_BOOKMARK_BAR_FOLDER_SUPERVISED);
#else
  return GetFolderIcon(gfx::VectorIconId::FOLDER_SUPERVISED, text_color);
#endif
}
