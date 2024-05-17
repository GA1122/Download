static bool NeedsHistoryItemRestore(WebFrameLoadType type) {
  return type == WebFrameLoadType::kBackForward || IsReloadLoadType(type);
}
