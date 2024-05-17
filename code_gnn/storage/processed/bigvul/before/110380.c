bool PluginModule::InitAsLibrary(const FilePath& path) {
  base::NativeLibrary library = base::LoadNativeLibrary(path, NULL);
  if (!library)
    return false;

  EntryPoints entry_points;

  if (!LoadEntryPointsFromLibrary(library, &entry_points) ||
      !InitializeModule(entry_points)) {
    base::UnloadNativeLibrary(library);
    return false;
  }
  entry_points_ = entry_points;
  library_ = library;
  return true;
}
