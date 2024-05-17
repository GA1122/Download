void PluginInstance::DidReceiveManualData(const char* buffer, int length) {
  DCHECK(load_manually_);
  if (plugin_data_stream_.get() != NULL) {
    plugin_data_stream_->DidReceiveData(buffer, length, 0);
  }
}
