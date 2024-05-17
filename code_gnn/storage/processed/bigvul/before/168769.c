void RenderWidgetHostImpl::PauseForPendingResizeOrRepaints() {
  TRACE_EVENT0("browser",
      "RenderWidgetHostImpl::PauseForPendingResizeOrRepaints");

  if (is_hidden())
    return;

  if (!repaint_ack_pending_ && !resize_ack_pending_)
    return;

  if (auto_resize_enabled_)
    return;

  if (!view_)
    return;

  const int kPaintMsgTimeoutMS = 167;

  TRACE_EVENT0("renderer_host", "RenderWidgetHostImpl::WaitForSurface");

  DCHECK(!is_hidden_) << "WaitForSurface called while hidden!";

  DCHECK(!in_get_backing_store_) << "WaitForSurface called recursively!";
  base::AutoReset<bool> auto_reset_in_get_backing_store(
      &in_get_backing_store_, true);

  if (!view_->ShouldContinueToPauseForFrame())
    return;

  TimeTicks start_time = TimeTicks::Now();
  TimeDelta time_left = TimeDelta::FromMilliseconds(kPaintMsgTimeoutMS);
  TimeTicks timeout_time = start_time + time_left;
  while (1) {
    TRACE_EVENT0("renderer_host", "WaitForSurface::WaitForSingleTaskToRun");
    if (ui::WindowResizeHelperMac::Get()->WaitForSingleTaskToRun(time_left)) {
      if (!view_->ShouldContinueToPauseForFrame())
        break;
    }
    time_left = timeout_time - TimeTicks::Now();
    if (time_left <= TimeDelta::FromSeconds(0)) {
      TRACE_EVENT0("renderer_host", "WaitForSurface::Timeout");
      break;
    }
  }
}
