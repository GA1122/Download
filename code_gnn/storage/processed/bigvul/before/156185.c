void RendererSchedulerImpl::DidAnimateForInputOnCompositorThread() {
  TRACE_EVENT0(TRACE_DISABLED_BY_DEFAULT("renderer.scheduler"),
               "RendererSchedulerImpl::DidAnimateForInputOnCompositorThread");
  base::AutoLock lock(any_thread_lock_);
  any_thread().fling_compositor_escalation_deadline =
      helper_.NowTicks() +
      base::TimeDelta::FromMilliseconds(kFlingEscalationLimitMillis);
}
