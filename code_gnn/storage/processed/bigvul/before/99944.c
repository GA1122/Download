bool PluginInstance::IsValidStream(const NPStream* stream) {
  std::vector<scoped_refptr<PluginStream> >::iterator stream_index;
  for (stream_index = open_streams_.begin();
          stream_index != open_streams_.end(); ++stream_index) {
    if ((*stream_index)->stream() == stream)
      return true;
  }

  return false;
}
