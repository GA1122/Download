void ReleasePermanentXIDDispatcher(gfx::PluginWindowHandle surface) {
  GtkNativeViewManager* manager = GtkNativeViewManager::GetInstance();
  manager->ReleasePermanentXID(surface);
}
