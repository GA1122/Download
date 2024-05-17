  void UnregisterAsObserver(ExecutionContext* context) {
    DCHECK(context);
    online_observer_handle_ = nullptr;
  }
