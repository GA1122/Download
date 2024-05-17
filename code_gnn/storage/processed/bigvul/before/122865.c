bool RenderProcessHostImpl::FastShutdownForPageCount(size_t count) {
  if (render_widget_hosts_.size() == count)
    return FastShutdownIfPossible();
  return false;
}
