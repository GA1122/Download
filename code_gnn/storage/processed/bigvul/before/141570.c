 TracingControllerImpl::~TracingControllerImpl() {
  base::trace_event::TraceLog::GetInstance()->RemoveAsyncEnabledStateObserver(
      this);
}
