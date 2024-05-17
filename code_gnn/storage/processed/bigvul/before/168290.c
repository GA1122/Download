ToolbarActionsBar* BrowserView::GetToolbarActionsBar() {
  BrowserActionsContainer* container =
      toolbar_button_provider_->GetBrowserActionsContainer();
  return container ? container->toolbar_actions_bar() : nullptr;
}
