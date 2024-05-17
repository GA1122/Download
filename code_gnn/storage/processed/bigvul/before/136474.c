void PropertyTreeManager::Finalize() {
  while (effect_stack_.size())
    CloseCcEffect();
}
