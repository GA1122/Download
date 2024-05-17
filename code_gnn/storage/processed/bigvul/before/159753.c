  explicit NetworkStateObserver(Document& document)
      : ContextLifecycleObserver(&document) {
    online_observer_handle_ = GetNetworkStateNotifier().AddOnLineObserver(
        this,
        TaskRunnerHelper::Get(TaskType::kNetworking, GetExecutionContext()));
  }
