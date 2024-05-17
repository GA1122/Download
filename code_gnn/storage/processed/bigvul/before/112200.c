aura::Window* StackingController::GetDefaultParent(aura::Window* window) {
  switch (window->type()) {
    case aura::client::WINDOW_TYPE_NORMAL:
    case aura::client::WINDOW_TYPE_POPUP:
      if (IsSystemModal(window))
        return GetSystemModalContainer(window);
      else if (IsWindowModal(window))
        return GetContainerForWindow(window->transient_parent());
      return always_on_top_controller_->GetContainer(window);
    case aura::client::WINDOW_TYPE_PANEL:
      return GetContainerById(internal::kShellWindowId_PanelContainer);
    case aura::client::WINDOW_TYPE_MENU:
      return GetContainerById(internal::kShellWindowId_MenuContainer);
    case aura::client::WINDOW_TYPE_TOOLTIP:
      return GetContainerById(
          internal::kShellWindowId_DragImageAndTooltipContainer);
    case aura::client::WINDOW_TYPE_CONTROL:
      return GetContainerById(
          internal::kShellWindowId_UnparentedControlContainer);
    default:
      NOTREACHED() << "Window " << window->id()
                   << " has unhandled type " << window->type();
      break;
  }
  return NULL;
}
