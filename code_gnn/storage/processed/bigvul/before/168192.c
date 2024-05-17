void BubbleManager::CloseBubblesOwnedBy(const content::RenderFrameHost* frame) {
  CloseAllMatchingBubbles(nullptr, frame, BUBBLE_CLOSE_FRAME_DESTROYED);
}
