void WebViewPlugin::ReplayReceivedData(WebPlugin* plugin) {
  if (!response_.isNull()) {
    plugin->didReceiveResponse(response_);
    size_t total_bytes = 0;
    for (std::list<std::string>::iterator it = data_.begin();
        it != data_.end(); ++it) {
      plugin->didReceiveData(it->c_str(), it->length());
      total_bytes += it->length();
    }
    UMA_HISTOGRAM_MEMORY_KB("PluginDocument.Memory", (total_bytes / 1024));
    UMA_HISTOGRAM_COUNTS("PluginDocument.NumChunks", data_.size());
  }
  if (finished_loading_) {
    plugin->didFinishLoading();
  }
  if (error_.get()) {
    plugin->didFailLoading(*error_);
   }
 }
