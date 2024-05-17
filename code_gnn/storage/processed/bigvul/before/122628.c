Extension::Extension(const FilePath& path,
                     scoped_ptr<extensions::Manifest> manifest)
    : manifest_version_(0),
      incognito_split_mode_(false),
      offline_enabled_(false),
      converted_from_user_script_(false),
      background_page_is_persistent_(true),
      allow_background_js_access_(true),
      manifest_(manifest.release()),
      finished_parsing_manifest_(false),
      is_storage_isolated_(false),
      launch_container_(extension_misc::LAUNCH_TAB),
      launch_width_(0),
      launch_height_(0),
      display_in_launcher_(true),
      display_in_new_tab_page_(true),
      wants_file_access_(false),
      creation_flags_(0) {
  DCHECK(path.empty() || path.IsAbsolute());
  path_ = MaybeNormalizePath(path);
}
