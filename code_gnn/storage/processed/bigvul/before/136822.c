String LocalDOMWindow::origin() const {
  return GetExecutionContext()->GetSecurityOrigin()->ToString();
}
