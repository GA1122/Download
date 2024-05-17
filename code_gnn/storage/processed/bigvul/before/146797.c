  explicit NetworkStateObserver(Document& document)
      : ContextLifecycleObserver(&document) {
    GetNetworkStateNotifier().AddOnLineObserver(
        this,
        TaskRunnerHelper::Get(TaskType::kNetworking, GetExecutionContext()));
  }
