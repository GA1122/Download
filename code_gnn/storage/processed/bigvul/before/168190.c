bool BubbleManager::CloseAllMatchingBubbles(
    BubbleController* bubble,
    const content::RenderFrameHost* owner,
    BubbleCloseReason reason) {
  DCHECK(!bubble || !owner);

  std::vector<std::unique_ptr<BubbleController>> close_queue;

  ManagerState original_state = manager_state_;
  manager_state_ = ITERATING_BUBBLES;
  for (auto i = controllers_.begin(); i != controllers_.end();) {
    if ((!bubble || bubble == (*i).get()) &&
        (!owner || (*i)->OwningFrameIs(owner)) && (*i)->ShouldClose(reason)) {
      close_queue.push_back(std::move(*i));
      i = controllers_.erase(i);
    } else {
      ++i;
    }
  }
  manager_state_ = original_state;

  for (auto& controller : close_queue) {
    controller->DoClose(reason);

    for (auto& observer : observers_)
      observer.OnBubbleClosed(controller->AsWeakPtr(), reason);
  }

  return !close_queue.empty();
}
