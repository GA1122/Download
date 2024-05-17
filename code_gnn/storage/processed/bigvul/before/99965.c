 PluginInstance::~PluginInstance() {
  CloseStreams();

  if (npp_ != 0) {
    delete npp_;
    npp_ = 0;
  }

  if (plugin_)
    plugin_->CloseInstance();
}
