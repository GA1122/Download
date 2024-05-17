void Document::ScheduleLayoutTreeUpdate() {
  DCHECK(!HasPendingVisualUpdate());
  DCHECK(ShouldScheduleLayoutTreeUpdate());
  DCHECK(NeedsLayoutTreeUpdate());

  if (!View()->CanThrottleRendering())
    GetPage()->Animator().ScheduleVisualUpdate(GetFrame());
  lifecycle_.EnsureStateAtMost(DocumentLifecycle::kVisualUpdatePending);

  TRACE_EVENT_INSTANT1(TRACE_DISABLED_BY_DEFAULT("devtools.timeline"),
                       "ScheduleStyleRecalculation", TRACE_EVENT_SCOPE_THREAD,
                       "data",
                       inspector_recalculate_styles_event::Data(GetFrame()));
  ++style_version_;
}
