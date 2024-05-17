void BrowserMainLoop::EndStartupTracing() {
  DCHECK(is_tracing_startup_for_duration_);

  is_tracing_startup_for_duration_ = false;
  TracingController::GetInstance()->StopTracing(
      TracingController::CreateFileEndpoint(
          startup_trace_file_,
          base::Bind(OnStoppedStartupTracing, startup_trace_file_)));
}
