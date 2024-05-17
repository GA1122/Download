void ScreenPositionController::SetBounds(aura::Window* window,
                                         const gfx::Rect& bounds,
                                         const gfx::Display& display) {
  DCHECK_NE(-1, display.id());
  if (!window->parent()->GetProperty(kUsesScreenCoordinatesKey)) {
    window->SetBounds(bounds);
    return;
  }

  if (!::wm::GetTransientParent(window) &&
      !ShouldStayInSameRootWindow(window)) {
    aura::Window* dst_root = Shell::GetInstance()
                                 ->window_tree_host_manager()
                                 ->GetRootWindowForDisplayId(display.id());
    DCHECK(dst_root);
    aura::Window* dst_container = NULL;
    if (dst_root != window->GetRootWindow()) {
      int container_id = window->parent()->id();
      DCHECK_GE(container_id, 0);
      if (!SystemModalContainerLayoutManager::IsModalBackground(window))
        dst_container = Shell::GetContainer(dst_root, container_id);
    }

    if (dst_container && window->parent() != dst_container) {
      aura::Window* focused = aura::client::GetFocusClient(window)->
          GetFocusedWindow();
      aura::client::ActivationClient* activation_client =
          aura::client::GetActivationClient(window->GetRootWindow());
      aura::Window* active = activation_client->GetActiveWindow();

      aura::WindowTracker tracker;
      if (focused)
        tracker.Add(focused);
      if (active && focused != active)
        tracker.Add(active);

      gfx::Point origin = bounds.origin();
      const gfx::Point display_origin = display.bounds().origin();
      origin.Offset(-display_origin.x(), -display_origin.y());
      gfx::Rect new_bounds = gfx::Rect(origin, bounds.size());

      window->SetBounds(new_bounds);

      dst_container->AddChild(window);

      MoveAllTransientChildrenToNewRoot(display, window);

      if (tracker.Contains(focused)) {
        aura::client::GetFocusClient(window)->FocusWindow(focused);
        ash::Shell::GetInstance()->set_target_root_window(
            focused->GetRootWindow());
      } else if (tracker.Contains(active)) {
        activation_client->ActivateWindow(active);
      }
    }
  }
  gfx::Point origin(bounds.origin());
  const gfx::Point display_origin = Shell::GetScreen()->GetDisplayNearestWindow(
      window).bounds().origin();
  origin.Offset(-display_origin.x(), -display_origin.y());
  window->SetBounds(gfx::Rect(origin, bounds.size()));
}
