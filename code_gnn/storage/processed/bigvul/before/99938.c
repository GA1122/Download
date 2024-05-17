void PluginInstance::CloseStreams() {
  in_close_streams_ = true;
  for (unsigned int index = 0; index < open_streams_.size(); ++index) {
    open_streams_[index]->Close(NPRES_USER_BREAK);
  }
  open_streams_.clear();
   in_close_streams_ = false;
 }
