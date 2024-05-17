void DevToolsWindow::AddCreationCallbackForTest(
    const CreationCallback& callback) {
  g_creation_callbacks.Get().push_back(callback);
}
