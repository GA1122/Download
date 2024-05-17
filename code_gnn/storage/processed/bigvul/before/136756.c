ExecutionContext* LocalDOMWindow::GetExecutionContext() const {
  return document_.Get();
}
