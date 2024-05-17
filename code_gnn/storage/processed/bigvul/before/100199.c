void PluginHelper::Observe(NotificationType type,
                           const NotificationSource& source,
                           const NotificationDetails& details) {
  DCHECK(CalledOnValidThread());
  DCHECK(type == NotificationType::CHROME_PLUGIN_UNLOADED);
  DCHECK(plugin_ == Source<ChromePluginLib>(source).ptr());

  delete this;
}
