 void Dispatcher::SetNotImplemented(const std::string& pattern) {
  mg_set_uri_callback(context_, (root_ + pattern).c_str(),
                      &SendNotImplementedError, NULL);
}
