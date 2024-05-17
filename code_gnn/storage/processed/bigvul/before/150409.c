  void Unregister(aura::Window* window) {
    window->RemoveObserver(this);
    DCHECK(policy_map_.find(window) != policy_map_.end());
    window->SetEventTargetingPolicy(policy_map_[window]);
    for (auto* child : window->children())
      Unregister(child);
  }
