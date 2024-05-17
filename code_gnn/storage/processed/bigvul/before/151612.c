static WebTaskRunner* GetTaskRunnerFor(const ResourceRequest& request,
                                       FetchContext& context) {
  if (!request.GetKeepalive())
    return context.GetLoadingTaskRunner().get();
  return Platform::Current()->CurrentThread()->Scheduler()->LoadingTaskRunner();
}
