ExecutionContext* PresentationConnection::getExecutionContext() const {
  if (!frame())
    return nullptr;
  return frame()->document();
}
