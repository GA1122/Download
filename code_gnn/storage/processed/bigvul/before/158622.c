FrameFetchContext::GetLoadingTaskRunner() {
  if (IsDetached())
    return FetchContext::GetLoadingTaskRunner();
  return GetFrame()->GetTaskRunner(TaskType::kNetworking);
}
