bool IsBackForwardLoadType(FrameLoadType type) {
  return type == kFrameLoadTypeBackForward ||
         type == kFrameLoadTypeInitialHistoryLoad;
}
