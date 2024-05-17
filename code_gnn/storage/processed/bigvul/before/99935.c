PluginHost *PluginHost::Singleton() {
  if (singleton_.get() == NULL) {
    singleton_ = new PluginHost();
  }

  DCHECK(singleton_.get() != NULL);
  return singleton_;
}
