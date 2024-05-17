WebSocketExperimentTask::WebSocketExperimentTask(
    const Config& config,
    net::CompletionCallback* callback)
    : config_(config),
      context_(ALLOW_THIS_IN_INITIALIZER_LIST(new Context())),
      method_factory_(ALLOW_THIS_IN_INITIALIZER_LIST(this)),
      callback_(callback),
      next_state_(STATE_NONE),
      last_websocket_error_(net::OK) {
}
