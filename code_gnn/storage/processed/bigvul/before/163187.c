  ~ScopedPluginRegister() {
    std::vector<WebPluginInfo> plugins;
    plugin_service_->GetInternalPlugins(&plugins);
    EXPECT_EQ(1u, plugins.size());
    plugin_service_->UnregisterInternalPlugin(plugins[0].path);
    plugin_service_->RefreshPlugins();

    plugins.clear();
    plugin_service_->GetInternalPlugins(&plugins);
    EXPECT_TRUE(plugins.empty());
  }
