void RendererSchedulerImpl::TaskQueuePolicy::AsValueInto(
    base::trace_event::TracedValue* state) const {
  state->SetBoolean("is_enabled", is_enabled);
  state->SetBoolean("is_paused", is_paused);
  state->SetBoolean("is_throttled", is_throttled);
  state->SetBoolean("is_blocked", is_blocked);
  state->SetBoolean("is_stopped", is_stopped);
  state->SetBoolean("use_virtual_time", use_virtual_time);
  state->SetString("priority", TaskQueue::PriorityToString(priority));
}
