bool SkiaOutputSurfaceImpl::Initialize() {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);

  task_sequence_ = dependency_->CreateSequence();

  weak_ptr_ = weak_ptr_factory_.GetWeakPtr();
  base::WaitableEvent event(base::WaitableEvent::ResetPolicy::MANUAL,
                            base::WaitableEvent::InitialState::NOT_SIGNALED);
  bool result = false;
  auto callback = base::BindOnce(&SkiaOutputSurfaceImpl::InitializeOnGpuThread,
                                 base::Unretained(this), &event, &result);
  ScheduleGpuTask(std::move(callback), std::vector<gpu::SyncToken>());
  event.Wait();
  return result;
}
