DocumentLifecycleNotifier* Document::lifecycleNotifier()
{
    return static_cast<DocumentLifecycleNotifier*>(ExecutionContext::lifecycleNotifier());
}
