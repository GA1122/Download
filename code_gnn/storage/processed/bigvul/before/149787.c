void LayerTreeHost::DidLoseCompositorFrameSink() {
  TRACE_EVENT0("cc", "LayerTreeHostInProcess::DidLoseCompositorFrameSink");
  DCHECK(task_runner_provider_->IsMainThread());
  SetNeedsCommit();
}
