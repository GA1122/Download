void PreloadPepperPlugins() {
  std::vector<PepperPluginInfo> plugins;
  ComputePepperPluginList(&plugins);
  for (const auto& plugin : plugins) {
    if (!plugin.is_internal) {
      base::NativeLibraryLoadError error;
      base::NativeLibrary library = base::LoadNativeLibrary(plugin.path,
                                                            &error);
      VLOG_IF(1, !library) << "Unable to load plugin "
                           << plugin.path.value() << " "
                           << error.ToString();

      (void)library;   
    }
  }
}
