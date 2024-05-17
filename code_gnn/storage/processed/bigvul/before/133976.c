bool IsUnRemovableDefaultApp(const std::string& id) {
  if (id == extension_misc::kChromeAppId ||
      id == extensions::kWebStoreAppId)
    return true;
#if defined(OS_CHROMEOS)
  if (id == file_manager::kFileManagerAppId || id == genius_app::kGeniusAppId)
    return true;
#endif
  return false;
 }
