void ProfilingService::AddProfilingClient(
    base::ProcessId pid,
    mojom::ProfilingClientPtr client,
    mojo::ScopedHandle memlog_pipe_sender,
    mojo::ScopedHandle memlog_pipe_receiver,
    mojom::ProcessType process_type,
    profiling::mojom::StackMode stack_mode) {
  connection_manager_.OnNewConnection(
      pid, std::move(client), std::move(memlog_pipe_sender),
      std::move(memlog_pipe_receiver), process_type, stack_mode);
}
