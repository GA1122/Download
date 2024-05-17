  void Run() {
    if (delegate_)
      delegate_->PluginDestroyed();

    delete webplugin_;
  }
