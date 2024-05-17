Bluetooth::Bluetooth(ExecutionContext* context)
    : ContextLifecycleObserver(context),
      PageVisibilityObserver(To<Document>(context)->GetPage()) {}
