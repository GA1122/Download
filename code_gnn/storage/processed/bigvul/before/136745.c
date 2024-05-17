void LocalDOMWindow::Dispose() {
  if (!GetFrame())
    return;

  RemoveAllEventListeners();
}
