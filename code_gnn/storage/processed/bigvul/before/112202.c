 bool IsWindowModal(aura::Window* window) {
  return window->transient_parent() &&
      window->GetProperty(aura::client::kModalKey) == ui::MODAL_TYPE_WINDOW;
}
