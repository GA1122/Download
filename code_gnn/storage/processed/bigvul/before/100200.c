PluginHelper::PluginHelper(ChromePluginLib* plugin) : plugin_(plugin) {
  DCHECK(CalledOnValidThread());
  registrar_.Add(this, NotificationType::CHROME_PLUGIN_UNLOADED,
                 Source<ChromePluginLib>(plugin_));
}
