void RendererSchedulerImpl::OnFirstMeaningfulPaint() {
  TRACE_EVENT0(TRACE_DISABLED_BY_DEFAULT("renderer.scheduler"),
               "RendererSchedulerImpl::OnFirstMeaningfulPaint");
  base::AutoLock lock(any_thread_lock_);
  any_thread().waiting_for_meaningful_paint = false;
  UpdatePolicyLocked(UpdateType::kMayEarlyOutIfPolicyUnchanged);
}
