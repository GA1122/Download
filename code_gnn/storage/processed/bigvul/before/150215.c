void TabletModeWindowManager::OnWindowBoundsChanged(
    aura::Window* window,
    const gfx::Rect& old_bounds,
    const gfx::Rect& new_bounds,
    ui::PropertyChangeReason reason) {
  if (!IsContainerWindow(window))
    return;

  auto* session = Shell::Get()->overview_controller()->overview_session();
  if (session)
    session->SuspendReposition();

  for (auto& pair : window_state_map_) {
    pair.second->UpdateWindowPosition(wm::GetWindowState(pair.first),
                                       false);
  }
  if (session)
    session->ResumeReposition();
}
