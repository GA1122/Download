WallpaperManagerBase::CustomizedWallpaperRescaledFiles::CreateCheckerClosure() {
  return base::Bind(&WallpaperManagerBase::CustomizedWallpaperRescaledFiles::
                        CheckCustomizedWallpaperFilesExist,
                    base::Unretained(this));
}
