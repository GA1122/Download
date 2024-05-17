    CustomizedWallpaperRescaledFiles(const base::FilePath& path_downloaded,
                                     const base::FilePath& path_rescaled_small,
                                     const base::FilePath& path_rescaled_large)
    : path_downloaded_(path_downloaded),
      path_rescaled_small_(path_rescaled_small),
      path_rescaled_large_(path_rescaled_large),
      downloaded_exists_(false),
      rescaled_small_exists_(false),
      rescaled_large_exists_(false) {
}
