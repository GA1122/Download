void IOHandler::ReadComplete(std::unique_ptr<ReadCallback> callback,
                             std::unique_ptr<std::string> data,
                             bool base64_encoded,
                             int status) {
  if (status == DevToolsIOContext::ROStream::StatusFailure) {
    callback->sendFailure(Response::Error("Read failed"));
    return;
  }
  bool eof = status == DevToolsIOContext::ROStream::StatusEOF;
  callback->sendSuccess(base64_encoded, std::move(*data), eof);
}
