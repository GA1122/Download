Response IOHandler::Close(const std::string& handle) {
  return io_context_->Close(handle) ? Response::OK()
      : Response::InvalidParams("Invalid stream handle");
}
