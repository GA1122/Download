void BubbleManager::CloseAllBubbles(BubbleCloseReason reason) {
  DCHECK_NE(reason, BUBBLE_CLOSE_ACCEPTED);
  DCHECK_NE(reason, BUBBLE_CLOSE_CANCELED);
  DCHECK(thread_checker_.CalledOnValidThread());
  DCHECK_NE(manager_state_, ITERATING_BUBBLES);
  CloseAllMatchingBubbles(nullptr, nullptr, reason);
}
