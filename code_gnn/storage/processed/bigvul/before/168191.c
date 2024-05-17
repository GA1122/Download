bool BubbleManager::CloseBubble(BubbleReference bubble,
                                BubbleCloseReason reason) {
  DCHECK(thread_checker_.CalledOnValidThread());
  DCHECK_NE(manager_state_, ITERATING_BUBBLES);
  return CloseAllMatchingBubbles(bubble.get(), nullptr, reason);
}
