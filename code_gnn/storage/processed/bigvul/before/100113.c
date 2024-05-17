gfx::Size BrowserActionsContainer::GetPreferredSize() {
  if (browser_action_views_.empty())
    return gfx::Size(0, 0);

  int width = std::max(ContainerMinSize(),
                       container_size_.width() - resize_amount_);
  int max_width = ClampToNearestIconCount(-1);   
  width = std::min(width, max_width);

  return gfx::Size(width, kButtonSize);
}
