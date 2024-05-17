  explicit NetworkStateObserver(Document& document)
      : ContextLifecycleObserver(&document) {
    online_observer_handle_ = GetNetworkStateNotifier().AddOnLineObserver(
        this, GetExecutionContext()->GetTaskRunner(TaskType::kNetworking));
  }
