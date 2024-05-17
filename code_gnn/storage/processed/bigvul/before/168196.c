void BubbleManager::UpdateAllBubbleAnchors() {
  DCHECK(thread_checker_.CalledOnValidThread());
  DCHECK_NE(manager_state_, ITERATING_BUBBLES);

  ManagerState original_state = manager_state_;
  manager_state_ = ITERATING_BUBBLES;
  for (auto& controller : controllers_)
    controller->UpdateAnchorPosition();
  manager_state_ = original_state;
}
