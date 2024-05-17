Extension::Extension(const FilePath& path, Location location)
    : incognito_split_mode_(false),
      location_(location),
      converted_from_user_script_(false),
      is_theme_(false),
      is_app_(false),
      is_storage_isolated_(false),
      launch_container_(extension_misc::LAUNCH_TAB),
      launch_width_(0),
      launch_height_(0),
      wants_file_access_(false) {
  DCHECK(path.empty() || path.IsAbsolute());
  path_ = MaybeNormalizePath(path);
}
