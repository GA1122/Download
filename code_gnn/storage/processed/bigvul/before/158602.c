FrameFetchContext::CreateResourceLoadingTaskRunnerHandle() {
  if (IsDetached()) {
    return scheduler::WebResourceLoadingTaskRunnerHandle::CreateUnprioritized(
        FetchContext::GetLoadingTaskRunner());
  }
  return GetFrame()
      ->GetFrameScheduler()
      ->CreateResourceLoadingTaskRunnerHandle();
}
