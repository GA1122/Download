void TracingControllerImpl::UnregisterTracingUI(TracingUI* tracing_ui) {
  auto it = tracing_uis_.find(tracing_ui);
  DCHECK(it != tracing_uis_.end());
  tracing_uis_.erase(it);
}
