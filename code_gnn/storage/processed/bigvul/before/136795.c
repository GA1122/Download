void LocalDOMWindow::cancelIdleCallback(int id) {
  if (Document* document = this->document())
    document->CancelIdleCallback(id);
}
