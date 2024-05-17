void PluginInstance::DidFinishManualLoading() {
  DCHECK(load_manually_);
  if (plugin_data_stream_.get() != NULL) {
    plugin_data_stream_->DidFinishLoading();
    plugin_data_stream_->Close(NPRES_DONE);
    plugin_data_stream_ = NULL;
  }
}
