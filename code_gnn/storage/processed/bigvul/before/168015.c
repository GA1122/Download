bool IsBackForwardLoadType(WebFrameLoadType type) {
  return type == WebFrameLoadType::kBackForward ||
         type == WebFrameLoadType::kInitialHistoryLoad;
}
