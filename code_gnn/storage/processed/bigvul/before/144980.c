void RenderWidgetHostViewAura::SetSelectionControllerClientForTest(
    std::unique_ptr<TouchSelectionControllerClientAura> client) {
  selection_controller_client_.swap(client);
  CreateSelectionController();
}
