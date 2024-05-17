void MimeHandlerViewContainer::PluginDidReceiveData(const char* data,
                                                    int data_length) {
  view_id_ += std::string(data, data_length);
}
