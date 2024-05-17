  void CreateCmdlineWallpapers() {
    base::ThreadRestrictions::ScopedAllowIO allow_io;
    wallpaper_dir_.reset(new base::ScopedTempDir);
    ASSERT_TRUE(wallpaper_dir_->CreateUniqueTempDir());
    wallpaper_manager_test_utils::CreateCmdlineWallpapers(
        *wallpaper_dir_, &wallpaper_manager_command_line_);
  }
