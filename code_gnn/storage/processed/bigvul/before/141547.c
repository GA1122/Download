void TracingControllerImpl::AddAgents() {
  tracing::TracedProcessImpl::GetInstance()->SetTaskRunner(
      base::SequencedTaskRunnerHandle::Get());

#if defined(OS_CHROMEOS)
  agents_.push_back(std::make_unique<CrOSTracingAgent>());
#elif defined(CAST_TRACING_AGENT)
  agents_.push_back(std::make_unique<CastTracingAgent>());
#endif

  auto* trace_event_agent = tracing::TraceEventAgent::GetInstance();
  trace_event_agent->AddMetadataGeneratorFunction(base::BindRepeating(
      &TracingControllerImpl::GenerateMetadataDict, base::Unretained(this)));
  if (delegate_) {
    trace_event_agent->AddMetadataGeneratorFunction(
        base::BindRepeating(&TracingDelegate::GenerateMetadataDict,
                            base::Unretained(delegate_.get())));
  }
}
