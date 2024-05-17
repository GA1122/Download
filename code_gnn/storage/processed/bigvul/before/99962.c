void PluginInstance::RemoveStream(PluginStream* stream) {
  if (in_close_streams_)
    return;

  std::vector<scoped_refptr<PluginStream> >::iterator stream_index;
  for (stream_index = open_streams_.begin();
       stream_index != open_streams_.end(); ++stream_index) {
    if (*stream_index == stream) {
      open_streams_.erase(stream_index);
      break;
    }
  }
}
