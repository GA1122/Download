void InputMethodController::ContextDestroyed(Document*) {
  Clear();
  composition_range_ = nullptr;
}
