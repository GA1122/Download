void GpuProcessHost::SetChildSurface(gpu::SurfaceHandle parent_handle,
                                     gpu::SurfaceHandle window_handle) {
#if defined(OS_WIN)
  constexpr char kBadMessageError[] = "Bad parenting request from gpu process.";
  if (!in_process_) {
    DCHECK(process_);

    DWORD parent_process_id = 0;
    DWORD parent_thread_id =
        GetWindowThreadProcessId(parent_handle, &parent_process_id);
    if (!parent_thread_id || parent_process_id != ::GetCurrentProcessId()) {
      LOG(ERROR) << kBadMessageError;
      return;
    }

    DWORD child_process_id = 0;
    DWORD child_thread_id =
        GetWindowThreadProcessId(window_handle, &child_process_id);
    if (!child_thread_id || child_process_id != process_->GetProcess().Pid()) {
      LOG(ERROR) << kBadMessageError;
      return;
    }
  }

  if (!gfx::RenderingWindowManager::GetInstance()->RegisterChild(
          parent_handle, window_handle)) {
    LOG(ERROR) << kBadMessageError;
  }
#endif
}
