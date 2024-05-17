  void UnregisterAsObserver(ExecutionContext* context) {
    DCHECK(context);
    GetNetworkStateNotifier().RemoveOnLineObserver(
        this, TaskRunnerHelper::Get(TaskType::kNetworking, context));
  }
