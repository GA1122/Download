void LocalDOMWindow::FrameDestroyed() {
  RemoveAllEventListeners();
  DisconnectFromFrame();
}
