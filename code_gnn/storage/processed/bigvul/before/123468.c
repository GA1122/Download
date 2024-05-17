ImageBitmapFactories& ImageBitmapFactories::from(EventTarget& eventTarget)
{
    if (DOMWindow* window = eventTarget.toDOMWindow())
        return fromInternal(*window);

    ASSERT(eventTarget.executionContext()->isWorkerGlobalScope());
    return WorkerGlobalScopeImageBitmapFactories::fromInternal(*toWorkerGlobalScope(eventTarget.executionContext()));
}
