bool RenderProcessHostImpl::FastShutdownForPageCount(size_t count) {
  if (GetActiveViewCount() == count)
    return FastShutdownIfPossible();
  return false;
}
