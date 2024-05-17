bool RenderViewImpl::isSmartInsertDeleteEnabled() {
#if defined(OS_MACOSX)
  return true;
#else
  return false;
#endif
}
