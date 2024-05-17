TracingControllerImpl* TracingControllerImpl::GetInstance() {
  DCHECK(g_tracing_controller);
  return g_tracing_controller;
}
