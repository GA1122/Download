gfx::Size GetMaximumSizeOfWindow(wm::WindowState* window_state) {
  DCHECK(window_state->CanMaximize() || window_state->CanResize());

  gfx::Size workspace_size =
      screen_util::GetMaximizedWindowBoundsInParent(window_state->window())
          .size();

  gfx::Size size = window_state->window()->delegate()
                       ? window_state->window()->delegate()->GetMaximumSize()
                       : gfx::Size();
  if (size.IsEmpty())
    return workspace_size;

  size.SetToMin(workspace_size);
  return size;
}
