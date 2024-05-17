base::ProcessHandle RenderProcessHostImpl::GetHandle() {
  if (run_renderer_in_process())
    return base::Process::Current().handle();

  if (!child_process_launcher_.get() || child_process_launcher_->IsStarting())
    return base::kNullProcessHandle;

  return child_process_launcher_->GetHandle();
}
