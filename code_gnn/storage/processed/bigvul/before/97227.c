void WebFrameLoaderClient::redirectDataToPlugin(Widget* pluginWidget) {
  plugin_widget_ = static_cast<WebPluginContainerImpl*>(pluginWidget);
  DCHECK(plugin_widget_.get());
}
