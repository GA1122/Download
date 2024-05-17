aura::Window* GetContainerForWindow(aura::Window* window) {
  aura::Window* container = window->parent();
  while (container && container->type() != aura::client::WINDOW_TYPE_UNKNOWN)
    container = container->parent();
  return container;
 }
