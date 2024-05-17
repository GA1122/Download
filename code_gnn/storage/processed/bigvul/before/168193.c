 void BubbleManager::FinalizePendingRequests() {
   if (manager_state_ == NO_MORE_BUBBLES)
    return;

  manager_state_ = NO_MORE_BUBBLES;
  CloseAllBubbles(BUBBLE_CLOSE_FORCED);
}
