bool TracingControllerImpl::StopTracing(
    const scoped_refptr<TraceDataEndpoint>& trace_data_endpoint,
    const std::string& agent_label) {
  if (!IsTracing() || drainer_)
    return false;
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

#if defined(OS_ANDROID)
  base::trace_event::TraceLog::GetInstance()->AddClockSyncMetadataEvent();
#endif

  tracing::TraceStartupConfig::GetInstance()->SetDisabled();
  trace_data_endpoint_ = std::move(trace_data_endpoint);
  is_data_complete_ = false;
  is_metadata_available_ = false;
  mojo::DataPipe data_pipe;
  drainer_.reset(
      new mojo::DataPipeDrainer(this, std::move(data_pipe.consumer_handle)));
  if (agent_label.empty()) {
    coordinator_->StopAndFlush(
        std::move(data_pipe.producer_handle),
        base::BindRepeating(&TracingControllerImpl::OnMetadataAvailable,
                            base::Unretained(this)));
  } else {
    coordinator_->StopAndFlushAgent(
        std::move(data_pipe.producer_handle), agent_label,
        base::BindRepeating(&TracingControllerImpl::OnMetadataAvailable,
                            base::Unretained(this)));
  }
  return true;
}
