ImageBitmapFactories& ImageBitmapFactories::From(EventTarget& event_target) {
  if (LocalDOMWindow* window = event_target.ToLocalDOMWindow())
    return FromInternal(*window);

  return ImageBitmapFactories::FromInternal(
      *To<WorkerGlobalScope>(event_target.GetExecutionContext()));
}
