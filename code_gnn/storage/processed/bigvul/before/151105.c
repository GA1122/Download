void DevToolsWindow::RemoveCreationCallbackForTest(
    const CreationCallback& callback) {
  for (size_t i = 0; i < g_creation_callbacks.Get().size(); ++i) {
    if (g_creation_callbacks.Get().at(i).Equals(callback)) {
      g_creation_callbacks.Get().erase(g_creation_callbacks.Get().begin() + i);
      return;
    }
  }
}
