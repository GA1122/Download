void PluginInstance::DidManualLoadFail() {
  DCHECK(load_manually_);
  if (plugin_data_stream_.get() != NULL) {
    plugin_data_stream_->DidFail();
    plugin_data_stream_ = NULL;
  }
}
