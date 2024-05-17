void Dispatcher::AddShutdown(const std::string& pattern,
                             base::WaitableEvent* shutdown_event) {
  mg_set_uri_callback(context_, (root_ + pattern).c_str(), &Shutdown,
                       shutdown_event);
 }
