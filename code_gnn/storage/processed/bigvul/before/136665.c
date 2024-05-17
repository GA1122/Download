bool IsReloadLoadType(FrameLoadType type) {
  return type == kFrameLoadTypeReload ||
         type == kFrameLoadTypeReloadBypassingCache;
}
