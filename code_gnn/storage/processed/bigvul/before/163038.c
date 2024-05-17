void ProfilingProcessHost::SetDumpProcessForTracingCallback(
    base::OnceClosure callback) {
  DCHECK(!dump_process_for_tracing_callback_);
  dump_process_for_tracing_callback_ = std::move(callback);
}
