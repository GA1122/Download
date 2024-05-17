void PluginHelper::DestroyAllHelpersForPlugin(ChromePluginLib* plugin) {
  NotificationService::current()->Notify(
      NotificationType::CHROME_PLUGIN_UNLOADED,
      Source<ChromePluginLib>(plugin),
      NotificationService::NoDetails());
}
