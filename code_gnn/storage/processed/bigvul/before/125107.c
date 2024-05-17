  void PluginsLoaded(const std::vector<webkit::WebPluginInfo>& plugins) {
    MessageLoop::current()->Quit();
  }
