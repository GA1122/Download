void RenderWidgetHostImpl::PauseForPendingResizeOrRepaints() {
  TRACE_EVENT0("browser",
      "RenderWidgetHostImpl::PauseForPendingResizeOrRepaints");

  if (!CanPauseForPendingResizeOrRepaints())
    return;

  WaitForSurface();
}
