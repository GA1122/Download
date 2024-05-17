DevToolsHttpHandler::~DevToolsHttpHandler() {
  TerminateOnUI(std::move(thread_), std::move(server_wrapper_),
                std::move(socket_factory_));
}
